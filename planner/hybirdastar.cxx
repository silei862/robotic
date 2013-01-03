/*
 * =====================================================================================
 *
 *       Filename:  hybirdastar.cxx
 *
 *    Description:  混合A＊算法实现
 *
 *        Version:  1.0
 *        Created:  2013年01月03日 09时19分39秒
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
#include <debug.h>
#include <exception.h>
#include <hybirdastar.h>
using namespace SlamLab;

HybirdAstar::HybirdAstar( DistanceMap& r_dmap, double safe_d )
{
	p_dmap = &r_dmap;
	_safe_distance = safe_d;
}

HybirdAstar::~HybirdAstar()
{
	nodelist_t::iterator it = _openlist.begin();
	// 清理open表
	for( ; it != _openlist.end() ; it++ )
		destroy_node( *it );
	// 清理close表
	it = _closelist.begin();
	for( ; it != _closelist.end() ; it++ )
		destroy_node( *it );
}

// ------------------ 对外接口 --------------------------------
// 设置起始状态
void
HybirdAstar::set_start( double x, double y , double th , double v , double w )
{
	if( !p_dmap->in( x, y) )
		throw_info( " out of map! " );
	// 填充状态信息：
	mixstate_t state;
	state._x = x;
	state._y = y;
	state._th = th;
	state._gpos = p_dmap->pos2sq( x , y );
	// 生成首节点：
	hanode_t* p_node = create_node( state , v , w );
	// 放入open表：
	_openlist.push_back( p_node );
}

// 设置目的坐标：
bool 
HybirdAstar::get_path( path_t& r_path )
{
	while( _openlist.size() )
	{
		hanode_t* p_node = _openlist.front();
		//_insert_closelist( p_node );
		_openlist.pop_front();
	}
}

void
HybirdAstar::set_destination( double x , double y )
{
	// 目标点合法性检查：
	if( !p_dmap->in( x , y ) )
		throw_info( " out of map! " );
	
	if( (*p_dmap)( x, y ) < _safe_distance )
		throw_info( " dangerous destination " );

	_dest_cell = p_dmap->pos2sq( x , y );
}


// ------------------ 内部接口 --------------------------------
// 节点管理：
HybirdAstar::hanode_t*
HybirdAstar::create_node()
{
	hanode_t* p_node = new hanode_t;
	if( NULL==p_node )
		throw_info( " alloc node failed! " );
	// 全初始化为0
	memset( (void*)p_node , 0 , sizeof( hanode_t ) );
	return p_node;
}

HybirdAstar::hanode_t*
HybirdAstar::create_node( HybirdAstar::mixstate_t& r_state, double v , double w )
{
	hanode_t* p_node= create_node();
	p_node->_state = r_state;
	p_node->_v =v;
	p_node->_w =w;

	return p_node;
}

void 
HybirdAstar::destroy_node( HybirdAstar::hanode_t* p_node )
{
	if( p_node )
	{
		delete p_node;
	}
}

// 启发函数计算：
void
HybirdAstar::_fill_heuristic( HybirdAstar::hanode_t* p_node )
{
	double x = p_node->_state._x;
	double y = p_node->_state._y;
	double th = p_node->_state._th;
	double v = p_node->_v;
	double w = p_node->_w;
	double depth = 0;
	// 存在父节点，则计算路径深度：
	if( p_node->p_parent )
	{
		hanode_t* pp = p_node->p_parent;
		double to_parent = _state_distance( p_node->_state, pp->_state ); 
		depth = to_parent + pp->_depth;
	}
	p_node->_depth = depth;
	p_node->_heuristic = 11;///////// to ///////////
	
}
// 计算距离：
double 
HybirdAstar::_state_distance( mixstate_t& r_st1 , mixstate_t& r_st2 )
{
	double dx = r_st1._x - r_st2._y;
	double dy = r_st1._y - r_st2._y;
	double dth = r_st1._th - r_st2._th;
	return sqrt( dx*dx + dy*dy + dth*dth );
}

double
HybirdAstar::_pos_distance( float_pos_t& r_pos1 , float_pos_t& r_pos2 )
{
	double dx = r_pos1._x - r_pos2._x;
	double dy = r_pos1._y - r_pos2._y;
	return sqrt( dx*dx + dy*dy );
}

// 节点列表操作：
inline void
HybirdAstar::_insert_openlist( hanode_t* p_node )
{
	_insert_list( _openlist , p_node );
}

inline void
HybirdAstar::_insert_closelist( hanode_t* p_node )
{
	_insert_list( _closelist , p_node );
}


void
HybirdAstar::_insert_list( nodelist_t& r_list , hanode_t* p_node )
{
	nodelist_t::iterator it = r_list.begin();
	// 搜索全部节点，排除相同点：
	for( ; it != r_list.end() ; it++ )
	{
		hanode_t* p_lstnode = *it;
		// 对位置进行比较
		grid_pos_t& r_lnpos = p_lstnode->_state._gpos;
		grid_pos_t& r_inpos = p_node->_state._gpos;
		/* if( r_lnpos == r_inpos )
			// 比较启发函数，小者替换大者：
			if( p_node->_heuristic < p_lstnode->_heuristic )
			{
				r_list.insert( it , p_node );
				r_list.erase( it );
				return;
			}
		*/
		// 找到合适的位置插入
		if( p_node->_heuristic < p_lstnode->_heuristic )
		{
			r_list.insert( it , p_node );
			return;
		}
	}
	// 如果无合适位置，直接插入表：
	r_list.insert( it , p_node );
}
