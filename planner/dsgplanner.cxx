/*
 * =====================================================================================
 *
 *       Filename:  dsgplanner.cxx
 *
 *    Description:  基于距离网格地图的全局导航类实现
 *
 *        Version:  1.0
 *        Created:  2012年12月29日 13时03分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <cmath>
#include <dsgplanner.h>

using namespace std;
using namespace SlamLab;

// ----------------- 对外接口 --------------------
DSGGuider::DSGGuider( DistanceMap& rdmap )
{
	p_dmap = &rdmap;
	// 设置默认参数：
	set_weight( def_k_dp , def_k_v , def_k_w , def_k_d2d );
	set_speed_interval( def_min_v , def_max_v );
	set_angular_speed_interval( def_min_w , def_max_w );
	set_delta_speed( def_delta_v , def_delta_w );
	set_time_delta( def_delta_t );
	set_safe_distance( def_safe_distance );
	set_max_listlen( def_max_listlen );
	// 设置默认深度：
	double ww = p_dmap->width();
	double hh = p_dmap->height();
	double depth = sqrt( ww*ww + hh*hh )*mul_depth;
	set_max_depth( depth );
	_tolarance = 1;
}

void DSGGuider::set_weight( double wdp , double wv , double ww , double wd2d )
{
	k_dp = wdp;
	k_v = wv;
	k_w = ww;
	k_d2d = wd2d;
}

void DSGGuider::set_speed_interval( double min_v , double max_v )
{
	_max_v = max_v;
	_min_v = min_v;
}

void DSGGuider::set_angular_speed_interval( double min_w , double max_w )
{
	_max_w = max_w;
	_min_w = min_w;
}

void DSGGuider::set_delta_speed( double dv , double dw )
{
	_delta_v = dv;
	_delta_w = dw;
}

void DSGGuider::set_time_delta( size_t dt )
{
	_delta_t = dt;
}

void DSGGuider::set_safe_distance( double sd )
{
	_safe_distance = sd;
}

void DSGGuider::set_distance_map( DistanceMap& rdmap )
{
	p_dmap = &rdmap;
}

void DSGGuider::set_max_depth( double depth )
{
	_max_depth = depth;
}

void DSGGuider::set_max_listlen( size_t maxlen )
{
	_max_listlen = maxlen;
}

void DSGGuider::set_start( double x ,double y , double th )
{
	// 初始化起始状态
	_start_state._x = x;
	_start_state._y = y;
	_start_state._th = th;
	_start_state._ctrl._tl = 0;
	_start_state._ctrl._v = 0.0;
	_start_state._ctrl._w = 0.0;

	// 生成首节点：
	exnode_t first_node;
	first_node._state = _start_state;
	first_node.p_parent = NULL;
	first_node._in_open = true;
	// 计算首节点的序值（启发函数值）
	_fill_heuristic_val( first_node );
	// 清空节点列表：
	_node_list.clear();
	// 将首节点加入open表：
	_node_list.push_back( first_node );
}

void DSGGuider::set_destination( double x ,double y, double th )
{
	_dest_state._x = x;
	_dest_state._y = y;
	_dest_state._th = th;
	_dest_state._ctrl._tl = 0;
	_dest_state._ctrl._v = 0.0;
	_dest_state._ctrl._w = 0.0;
}
// 获取可行控制向量组：
bool DSGGuider::get_controls( ctrlgroup_t& ctrls )
{
	while( true )
	{
		bool all_closed ;
		exnode_t& node = _get_firstnode_in_open( all_closed );
		// 如果全部被展开过则结束：
		if( all_closed )
			break;
		// 将该节点标记为false（移出open表)
		node._in_open = false;
		INFO_VAR( _node_list.size() );
		INFO_VAR( node._state._x );
		INFO_VAR( node._state._y );
		// 检查是否到达目的：
		if( _dest_achived( node ) )
		{
			_ctrlvectors_gen( node , ctrls );
			return true;
		}
		// 获取该节点所有子节点并插入open表：
		nodevector_t childnodes;
		_childnode_gen( node , childnodes );
		for( size_t i = 0 ; i < childnodes.size() ; i ++ ) 
			if( childnodes[i]._depth < _max_depth )
				_insert_list( childnodes[i] );
		if( _node_list.size() > _max_listlen )
			_node_list.pop_back();

	}
	ctrls.clear();
	return false;
}

exnode_t& DSGGuider::_get_firstnode_in_open( bool& all_closed )
{
	// 遍历表中所有节点，找出标志为open的点
	nodelist_t::iterator it = _node_list.begin();
	for( ;it != _node_list.end() ; it++ )
		if( it->_in_open )
		{
			all_closed = false;
			return *it;
		}
	all_closed = true;
	return *(_node_list.begin());
}

bool DSGGuider::_dest_achived( exnode_t& node )
{
	// 计算位置差
	double err = _pos_distance( node._state , _dest_state );
	if( err <= _tolarance )
		return true;
	else
		return false;
}

void DSGGuider::_childnode_gen( exnode_t& node , nodevector_t& chnodes )
{
	// 清除孩子节点组：
	chnodes.clear();
	// 尝试改变速度角速度，进行迁移计算：
	for( int mul_v = -2; mul_v <=2 ; mul_v++ )
		for( int mul_w = -2; mul_w <=2 ; mul_w++ )
		{
			// 在父节点控制状态下加以变化：
			state_t state = node._state;
			state._ctrl._v += mul_v*_delta_v;
			state._ctrl._w += mul_w*_delta_w;
			// 速度值限定：
			if( state._ctrl._v < _min_v || state._ctrl._v > _max_v )
				continue;
			if( state._ctrl._w < _min_w || state._ctrl._w > _max_w )
				continue;
			// 进行迁移计算：
			_state_transfer( state );
			// 可行性检查
			if( _state_applicable( state ) )
			{
				// 填充孩子节点：
				exnode_t child_node;
				child_node._state = state;
				child_node.p_parent = &node;
				child_node._in_open = true;
				_fill_heuristic_val( child_node );
				// 加入节点组：
				chnodes.push_back( child_node );
			}
		}
}

// 状态迁移计算：
void DSGGuider::_state_transfer( state_t& state )
{
	// 取出控制量
	double v = state._ctrl._v;
	double w = state._ctrl._w;
	// 取出状态量
	double x = state._x;
	double y = state._y;
	double th = state._th;
	// 位置变化量
	double abs_dx = 0;
	double abs_dy = 0;
	double c_sz = p_dmap->cell_size();

	int t_mul = 0;
	do
	{
		t_mul++;
		double t = _delta_t*t_mul/1000;
		if( 0 == w )
		{
			x += v*t*cos(th);
			y += v*t*sin(th);
		}
		else
		{
			x +=v*sin(w*t+th)/w;
			y -=v*cos(w*t+th)/w;
			th += w*t;
		}
		abs_dx = fabs( x - state._x );
		abs_dy = fabs( y - state._y );
	}while( abs_dx < c_sz || abs_dy < c_sz );

	// 更新状态量,控制量：
	state._x = x;
	state._y = y;
	state._th = th;
	state._ctrl._tl = _delta_t*t_mul;
	
}

bool DSGGuider::_state_applicable( state_t& state )
{
	// 超出地图？
	if( !p_dmap->in( state._x , state._y ) )
		return false;
	// 状态可行？
	if( (*p_dmap)( state._x , state._y )._d > _safe_distance )
		return true;
	else
		return false;
}

// 控制向量产生器：
void DSGGuider::_ctrlvectors_gen( exnode_t& node , ctrlgroup_t& ctrls )
{
	ctrlgroup_t revgrp;
	exnode_t* p_node = &node;
	while( p_node->p_parent )
	{
		// 根据当前节点生成控制向量
		control_t ctrl = p_node->_state._ctrl;
		// 保存到反转控制向量组：
		revgrp.push_back( ctrl );
		// 移动到父节点：
		p_node = p_node->p_parent;
	}
	// 控制向量大小调整：
	ctrls.resize( revgrp.size() );
	// 反转后得到控制向量组：
	for( size_t i = 0 ; i < ctrls.size() ; i++ )
		ctrls[ i ] = revgrp[ revgrp.size() - i -1 ];
}

// ------------------ 计算各类值 -------------------------
void  DSGGuider::_fill_heuristic_val( exnode_t& node )
{
	double depth = 0;
	// 如果有父节点则以父节点状态值作为基础
	if( node.p_parent )
	{
		exnode_t* pp = node.p_parent;
		// 获取父节点路径深度：
		depth = pp->_depth + _state_distance( pp->_state , node._state );
	}
	// 保存状态距离和
	node._depth = depth;
	// 获取启发值所需参数：
	double abs_w = fabs( node._state._ctrl._w );
	double abs_pv = ( _max_v - node._state._ctrl._v )/(_max_v - _min_v);
	double d2d = _pos_distance(	node._state , _dest_state );
	// 计算启发函数值：
	node._heuritic = k_dp*depth + k_v*abs_pv + k_w*abs_w + k_d2d*d2d; 
}

double DSGGuider::_state_distance( state_t& st1 , state_t& st2 )
{
	double dx = st1._x - st2._y;
	double dy = st1._y - st2._y;
	double dth = st1._th - st2._th;
	return sqrt( dx*dx + dy*dy + dth*dth );
}

double DSGGuider::_pos_distance( state_t& st1 , state_t& st2 )
{
	double dx = st1._x - st2._x;
	double dy = st1._y - st2._y;
	return sqrt( dx*dx + dy*dy );
}

// ----------------- 节点插入表 -----------------------
void DSGGuider::_insert_list( exnode_t& node )
{
	nodelist_t::iterator it = _node_list.begin();
	// 遍历整个列表找到合适位置并插入：
	for( ; it != _node_list.end(); it++ )
		if( node._heuritic < it->_heuritic )
		{
			_node_list.insert( it , node );
			return;
		}
	// 无合适位置则添加到表尾：
	_node_list.push_back( node );
}
