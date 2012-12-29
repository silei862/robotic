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
DSGGuider::DSGGuider()
{

}

void DSGGuider::init( state_t& cur, state_t& dest )
{
	_cur_state = cur;
	// 根据传入状态生成首节点：
	exnode_t first_node;
	first_node._state = cur;
	first_node.p_parent = NULL;
	// 计算首节点的序值（启发函数值）
	_fill_heuristic_val( first_node );
	// 清空open、close表：
	_open_list.clear();
	_close_list.clear();
	// 将首节点加入open表：
	_open_list.push_back( first_node );
}

void DSGGuider::set_dest( state_t& state )
{
	_dest_state = state;
}
// 获取可行控制向量组：
bool DSGGuider::get_controls( ctrlgroup_t& ctrls )
{
	while( !_open_list.empty() )
	{
		// 从open表中取出一个节点
		exnode_t node = _open_list.front();
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
			_insert_openlist( childnodes[i] );
		// 将该节点插入close表并从open表中弹出
		_insert_closelist( node );
		_open_list.pop_front();
	}
	ctrls.clear();
	return false;
}

bool DSGGuider::_dest_achived( exnode_t& node )
{
	// 计算位置差
	double err = _pos_distance( node._state._pos , _dest_state._pos );
	if( err <= _tolarance )
		return true;
	else
		return false;
}

void DSGGuider::_childnode_gen( exnode_t& node , nodevector_t& chnodes )
{
	// 计算速度、角速度搜索空间中心：
	int vc = node._state._speed._v / _delta_v;
	int wc = node._state._speed._w / _delta_w;
	// 计算边界：
	int min_v_mul = vc - _v_delta_mul;
	int min_w_mul = wc - _w_delta_mul;
	int max_v_mul = vc + _v_delta_mul;
	int max_w_mul = wc + _w_delta_mul;
	// 边界限定：
	if( min_v_mul < -1*_v_mul )
		min_v_mul = -1*_v_mul;
	if( min_w_mul < -1*_w_mul )
		min_w_mul = -1*_w_mul;
	if( max_v_mul > _v_mul )
		max_v_mul = _v_mul;
	if( max_w_mul > _w_mul )
		max_w_mul = _w_mul;
	// 清空节点向量组：
	chnodes.clear();
	// 在线速度、角速度空间展开节点：
	for( int v_mul = min_v_mul ; v_mul <= max_v_mul ; v_mul++ )
		for( int w_mul = min_w_mul ; w_mul <=max_w_mul ; w_mul++ )
		{
			state_t state = node._state;
			// 迁移线速度、角速度状态：
			state._speed._v = v_mul*_delta_v;
			state._speed._w = w_mul*_delta_w;
			// 进行状态迁移计算：
			_state_transfer( state );
			// 可行性检查：
			if( _state_applicable( state ) )
			{
				// 填充孩子节点成员：
				exnode_t childnode;
				childnode._state = state;
				childnode.p_parent = &node;
				_fill_heuristic_val( childnode );
				// 放入节点组：
				chnodes.push_back( childnode );
			}
		}
}

void DSGGuider::_state_transfer( state_t& state )
{
	double v = state._speed._v;
	double w = state._speed._w;
	double th = state._th;
	double delta_th = w*_step;
	double delta_x = v/w*cos( th );
	double delta_y = v/w*sin( th );
	// 迁移状态空间：
	state._th = th;
	state._pos._x += delta_x;
	state._pos._y += delta_y;
}

bool DSGGuider::_state_applicable( state_t& state )
{
	// 超出地图？
	if( !p_dmap->in( state._pos ) )
		return false;
	// 状态可行？
	if( (*p_dmap)( state._pos )._d < _safe_distance )
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
		// 根据当前节点和其父节点参数生成控制节点
		control_t ctrl;
		exnode_t* pp = p_node->p_parent;
		// 以父节点速度作为控制量：
		ctrl._ctl = pp->_state._speed;
		// 以当前节点状态量作为目标坐标、姿态
		ctrl._dest_pos = p_node->_state._pos;
		ctrl._dest_th = p_node->_state._th;
		// 加入反转控制向量组
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
	double abs_dv = 0;
	// 如果有父节点则以父节点状态值作为基础
	if( node.p_parent )
	{
		exnode_t* pp = node.p_parent;
		// 获取父节点路径深度：
		depth = pp->_depth + _state_distance( pp->_state , node._state );
		// 计算与父节点的速度差与状态差：
 		abs_dv = fabs( pp->_state._speed._v - node._state._speed._v );
	}
	// 保存状态距离和
	node._depth = depth;
	// 获取启发值所需参数：
	double abs_w = fabs( node._state._speed._w );
	double d2d = _pos_distance(	node._state._pos , _dest_state._pos );
	// 计算启发函数值：
	node._heuritic = k_dp*depth + k_v*abs_dv + k_w*abs_w + k_d2d*d2d; 
}

double DSGGuider::_state_distance( state_t& st1 , state_t& st2 )
{
	double dx = st1._pos._x - st2._pos._x;
	double dy = st1._pos._y - st2._pos._y;
	double dth = st1._th - st2._th;
	return sqrt( dx*dx + dy*dy + dth*dth );
}

double DSGGuider::_pos_distance( position_t& pos1 , position_t& pos2 )
{
	double dx = pos1._x - pos2._x;
	double dy = pos1._y - pos2._y;
	return sqrt( dx*dx + dy*dy );
}

// ----------------- 节点插入表 -----------------------
inline void DSGGuider::_insert_openlist( exnode_t& node )
{
	_insert_list( _open_list , node );
}

inline void DSGGuider::_insert_closelist( exnode_t& node )
{
	_insert_list( _close_list , node );
}

void DSGGuider::_insert_list( nodelist_t& list , exnode_t& node )
{
	nodelist_t::iterator it = list.begin();
	// 遍历整个列表找到合适位置并插入：
	for( ; it != list.end(); it++ )
		if( node._order_val < it->_order_val )
		{
			list.insert( it , node );
			return;
		}
	// 无合适位置则添加到表尾：
	list.push_back( node );
}
