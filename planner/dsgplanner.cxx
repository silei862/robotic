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
#include <exception.h>
#include <misc.h>
#include <dsgplanner.h>

using namespace std;
using namespace SlamLab;

// ----------------- 可行位置常量组 -------------
const DSGGuider::statetrans_t 
DSGGuider::strans[DIR_NUM][OP_NUM] = \
						 {{{ 1 , 0 , 0 },{ 1 ,-1 , 7 },{ 1 , 1 , 1 }},	//0
						  {{ 1 , 1 , 1 },{ 1 , 0 , 0 },{ 0 , 1 , 2 }},	//1
						  {{ 0 , 1 , 2 },{ 1 , 1 , 1 },{-1 , 1 , 3 }},	//2
						  {{-1 , 1 , 3 },{ 0 , 1 , 2 },{-1 , 0 , 4 }},	//3
						  {{-1 , 0 , 4 },{-1 , 1 , 3 },{-1 ,-1 , 5 }},	//4
						  {{-1 ,-1 , 5 },{-1 , 0 , 4 },{ 0 ,-1 , 6 }},	//5
						  {{ 0 ,-1 , 6 },{-1 ,-1 , 5 },{ 1 ,-1 , 7 }},	//6
						  {{ 1 ,-1 , 7 },{ 0 ,-1 , 6 },{ 1 , 0 , 0 }}};	//7

// ----------------- 对外接口 --------------------

DSGGuider::DSGGuider( DistanceMap& r_dmap , double safe_distance )
{
	p_dmap = &r_dmap;
	_safe_distance = safe_distance;
	// 设置默认参数：
	set_heuristic_para( def_h_depth , def_h_destination, def_h_dvalue );
	set_nodelist_len( def_max_node_num );
}

void
DSGGuider::set_start( double x , double y , double th )
{
	// 位置合法性检测：
	if( !p_dmap->in( x, y ) )
		throw_info( " Position out of map!" );
	if( (*p_dmap)( x , y)._d <= _safe_distance )
		throw_info( " Dangerous start point!" );
	_nodelist.clear();
	// 根据传入参数生成首节点：
	exnode_t node;
	node._pos = p_dmap->pos2sq( x , y );
	node._direction = _dir_trans( th );
	node._inopen = true;
	node._heuristic = 0;
	node._depth = 0;
	node.p_parent = NULL;
	// 放入节点列表：
	_nodelist.push_back( node );
}

void
DSGGuider::set_destination( double x , double y )
{
	// 位置合法性检查：
	if( !p_dmap->in( x , y ) )
		throw_info( " Position out of map!" );
	if( (*p_dmap)( x , y)._d <= _safe_distance )
		throw_info( " Dangerous start point!" );

	_dest_cell = p_dmap->pos2sq( x , y );	
}

bool
DSGGuider::get_path( path_t& r_path )
{
	r_path.clear();
	return _get_path( r_path );
}

void 
DSGGuider::set_heuristic_para( double k_depth , double k_dest , double k_dval )
{
	h_depth = k_depth;
	h_destination = k_dest;
	h_dval = k_dval;
}

void
DSGGuider::set_nodelist_len( size_t len )
{
	_max_node_num = len;
}

// ----------------- 内部函数 ------------------
uint8_t
DSGGuider::_dir_trans( double rad )
{
	double th = rad_hold( rad );
	// 逆时针旋转一次
	th += PI/8;
	// 计算方位量化值：
	int int_dir = int( th*4/PI );
	if( int_dir < 0 )
		int_dir += 7;
	return uint8_t( int_dir );
}

bool
DSGGuider::_get_path( path_t& r_path )
{
	while( true )
	{
		bool all_closed;
		exnode_t& node = _get_first_opennode( all_closed );
		if( all_closed )
			break;
		// 检查目标状态：
		if( node._pos == _dest_cell )
		{
			// 根据节点生成路径：
			_node_to_path( node , r_path );
			return true;
		}
		node._inopen = false;
		// 展开节点：

#ifdef _EXPAND_ALL_NODE_
		for( int j = -1 ; j<=1 ; j++ )	//***
			for( int i = -1 ; i <=1 ; i++ )	//***
#endif //_EXPAND_ALL_NODE_ 

#ifndef _EXPAND_ALL_NODE_ 				
		uint8_t dir = node._direction;		//###
		for(size_t i = 0 ; i< OP_NUM ; i++ ) //###
#endif //_EXPAND_ALL_NODE_

		{
				// 中心不计算：
#ifdef _EXPAND_ALL_NODE_
				if( j == 0 && i == 0 )	//***
					continue;			//***
#endif
				// 计算相邻格坐标：
#ifndef _EXPAND_ALL_NODE_
				int x = int(node._pos._x) + strans[dir][i]._dx; //###
				int y = int(node._pos._y) + strans[dir][i]._dy; //###
#endif

#ifdef _EXPAND_ALL_NODE_
				int x = int( node._pos._x ) + i;	//***
				int y = int( node._pos._y ) + j;	//***
#endif
				// 坐标合法性检查：
				if( x < 0 || y < 0 )
					continue;
				if( ! p_dmap->in( size_t(x) , size_t(y) ) )
					continue;
				if( (*p_dmap)( size_t(x) , size_t(y) )._d <= _safe_distance )
					continue;
				// 根据合法坐标生成子节点：
				exnode_t newnode;
				newnode._inopen = true;
				newnode._pos._x = size_t( x );
				newnode._pos._y = size_t( y );

#ifndef _EXPAND_ALL_NODE_
				newnode._direction = strans[dir][i]._dir; //###
#endif	//_EXPAND_ALL_NODE_

				newnode.p_parent = &node;
				_fill_heuristic( newnode );
				_insert_node( newnode );
				// 列表深度检查：
				if( _nodelist.size() > _max_node_num )
					return false;
		}
	}
	return false;
}

DSGGuider::exnode_t&
DSGGuider::_get_first_opennode( bool& allclosed  )
{
	exnodelist_t::iterator it = _nodelist.begin();
	// 查找第一个未扩展的节点：
	for( ; it != _nodelist.end() ; it++ )
	{
		if( it->_inopen )
		{
			allclosed = false;
			return *it;
		}
	}
	// 未找到
	allclosed = true;
	return *( _nodelist.begin() );
}

void
DSGGuider::_node_to_path( exnode_t& r_node , path_t& r_path )
{
	exnode_t* p_node = &r_node;
	path_t reverse_path;
	while( p_node->p_parent )
	{
		// 坐标变换：
		float_pos_t fpos = p_dmap->cell_coord( p_node->_pos );
		reverse_path.push_back( fpos );
		// 移动到父节点：
		p_node = p_node->p_parent;
	}
	// 反转坐标列表：
	r_path.resize( reverse_path.size() );
	for( size_t i = 0 ; i < r_path.size() ; i++ )
		r_path[ i ] = reverse_path[ r_path.size()-i-1 ];
}

void
DSGGuider::_fill_heuristic( exnode_t& r_node )
{
	r_node._depth = 0;
	// 计算深度：
	if( r_node.p_parent )
		r_node._depth = r_node.p_parent->_depth + 1;
	// 计算与目标距离：
	double dx = double( _dest_cell._x ) - double( r_node._pos._x );
	double dy = double( _dest_cell._y ) - double( r_node._pos._y );
	double dd = sqrt( dx*dx + dy*dy );
	// 获取节点对应网格处距离值：
	double dval = p_dmap->max_distance() - (*p_dmap)( r_node._pos )._d;
	r_node._heuristic = r_node._depth*h_depth + dd*h_destination + dval*h_dval;
}

void
DSGGuider::_insert_node( exnode_t& r_node )
{
	// 如果表为空，直接插入：
	if( _nodelist.size() == 0 )
	{
		_nodelist.push_back( r_node );
		return;
	}
	exnodelist_t::iterator it = _nodelist.begin();
	// 重复性检查：
	for( ; it != _nodelist.end() ; it++ )
		if( it->_pos == r_node._pos )
			return;
	// 插入表：
	it = _nodelist.begin();
	for( ; it != _nodelist.end(); it++ )
		if( it->_heuristic > r_node._heuristic )
		{
			_nodelist.insert( it , r_node );
			return;
		}
	// 无合适位置则放入尾部：
	_nodelist.push_back( r_node );
}
