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
#include <misc.h>
#include <hybirdastar.h>
using namespace SlamLab;

HybirdAstar::HybirdAstar( DistanceMap& r_dmap, double safe_d )
{
	p_dmap = &r_dmap;
	_safe_distance = safe_d;
	// 计算状态迁移步长：
	_step_len = 1.1*p_dmap->cell_size(); 
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
	state._v = v;
	state._w = w;
	state._gpos = p_dmap->pos2sq( x , y );
	// 生成首节点：
	hanode_t* p_node = create_node( state , NULL);
	// 放入open表：
	_openlist.push_back( p_node );
}

// 获取路径：
bool 
HybirdAstar::get_path( path_t& r_path )
{
	while( _openlist.size() )
	{
		hanode_t* p_node = _openlist.front();
		// 目标到达检查：
		if( p_node->_state._gpos == _dest_cell )
		{
			r_path.clear();
			_node_to_path( p_node , r_path );
			return true;
		}
		_insert_closelist( p_node );
		_openlist.pop_front();
		// 展开节点：
		nodevector_t chnodes;
		_expand_node( p_node ,chnodes ); 		
		for( size_t i = 0 ; i < chnodes.size() ; i++ )
			_insert_openlist( chnodes[i] );
	}
	return false;
}

// 设置目的坐标：
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

// ------------------ 参数设置 --------------------------------


// ------------------ 内部接口 --------------------------------
// 节点转路径：
void 
HybirdAstar::_node_to_path( hanode_t* p_node , path_t& r_path )
{
	path_t re_path;
	re_path.clear();
	hanode_t* p_rn = p_node;
	// 根据节点生成路径：
	while( p_rn->p_parent )
	{
		float_pos_t pos;
		pos._x = p_rn->_state._x;
		pos._y = p_rn->_state._y;
		re_path.push_back( pos );
	}
	// 反转路径：
	size_t path_size = re_path.size();
	r_path.resize( path_size );
	for( size_t i =0 ; i < path_size ; i++ )
		r_path[i] = re_path[ path_size - i -1 ];
}

// 节点展开：
void
HybirdAstar::_expand_node( hanode_t* p_node , nodevector_t& nodes )
{
	// 以当前状态为基础，进行迁移计算：
	mixstate_t state = p_node->_state;
	grid_pos_t center_pos = state._gpos;
	double min_v = state._v - _max_delta_v;
	if( min_v < _min_v )
		min_v = _min_v;
	double max_v = state._v + _max_delta_v;
	if( max_v > _max_v )
		max_v = _max_v;
	double min_w = state._w - _max_delta_w;
	if( min_w < _min_w )
		min_w = _min_w;
	double max_w = state._w + _max_delta_w;
	if( max_w > _max_w )
		max_w = _max_w;
	// 尝试不同速度、角速度下的状态迁移：
	for( double v = min_v ; v <= max_v ; v +=_delta_v )
		for( double w = min_w ; w <= max_w ; w +=_delta_w )
		{
			_transfer_state( state , v , w );
			// 合法性检查:
			// 条件：在地图内、在相邻格中、该位置安全
			if( p_dmap->in( state._x , state._y ) \
					&& _in_neighbor(center_pos , state._gpos ) \
					&& (*p_dmap)(state._x , state._y) > _safe_distance )
			{
				hanode_t* p_newnode = create_node( state , p_node );
				nodes.push_back( p_newnode );
			}
		}
}

// 状态迁移计算：
void 
HybirdAstar::_transfer_state( mixstate_t& r_state , double v , double w )
{
	static const size_t NS=2;
	// 迁移准备：
	double x = r_state._x;
	double y = r_state._y;
	double th = rad_hold( r_state._th );
	double delta_t;
	// 获取网格边长：
	double cell_size = p_dmap->cell_size();
	// 计算步长：
	if( th > -1*PI/4  && th < PI/4 )
	{ 	// x变化为正：
		if( 0 == w )
			delta_t = _step_len/(v*cos(th));
		else
			delta_t = (asin( _step_len*w/v + sin( th )) - th )/w;
	}
	else if( th >= PI/4 && th< 3*PI/4 )
	{	// y变化为正
		if( 0 == w )
			delta_t = _step_len/(v*sin(th));
		else
			delta_t = (acos( -1*_step_len*w/v + cos( th )) - th )/w;
	}
	else if((th >= 3*PI/4 && th < PI )||( th >= -1*PI && th < -3*PI/4 ))
	{	// x变化为负
		if( 0 == w )
			delta_t = -1*_step_len/(v*cos(th));
		else
			delta_t = ( asin( -1*_step_len*w/v + sin( th )) - th )/w;
	}
	else if( th >= -3*PI/4 && th < -1*PI/4 )
	{	// y变化为负
		if( 0 == w )
			delta_t = -1*_step_len/(v*sin(th));
		else
			delta_t = ( acos( _step_len*w/v + cos( th )) - th )/w;
	}

	// 迁移状态：
	if( 0 == w )
	{
		x += v*delta_t*cos( th );
		y += v*delta_t*sin( th );
	}
	else
	{
		x += ( sin( w*delta_t + th ) - sin( th ) )*v/w;
		y += ( cos( w*delta_t + th ) - cos( th ) )*v*(-1)/w;
		th += w*delta_t;
	}
	// 填充新状态：
	r_state._gpos = p_dmap->pos2sq( x , y);
	r_state._x = x;
	r_state._y = y;
	r_state._th = th;
	r_state._v = v;
	r_state._w = w;
}

// 邻格检查：
inline bool
HybirdAstar::_in_neighbor( grid_pos_t& r_centerpos , grid_pos_t& r_pos )
{
	int di = r_centerpos._x - r_pos._x;
	int dj = r_centerpos._y - r_pos._y;
	return ( di>= -1 && di <= 1 && dj >= -1 && dj <= 1 );
}

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
HybirdAstar::create_node( HybirdAstar::mixstate_t& r_state, hanode_t* p_parent )
{
	hanode_t* p_node= create_node();
	p_node->_state = r_state;
	p_node->p_parent = p_parent;
	_fill_heuristic( p_node );
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
	// 计算与目标角度差：
	double delta_ag = _to_destangular( p_node->_state );
	// 计算与目标的距离：
	double to_dest = _to_destdistance( p_node->_state );
	p_node->_heuristic = depth*h_depth + delta_ag*h_theta + to_dest*h_todest;
	
}

// 计算与目标的位置距离：
double 
HybirdAstar::_to_destdistance( mixstate_t& r_state )
{
	float_pos_t dest_pos = p_dmap->cell_coord( _dest_cell );
	double dx = dest_pos._x - r_state._x;
	double dy = dest_pos._y - r_state._y;
	return sqrt( dx*dx + dy*dy );
}

// 计算与目标连线的角度差值：
double 
HybirdAstar::_to_destangular( mixstate_t& r_state )
{
	// 计算目标方位角度：
	float_pos_t dest_pos = p_dmap->cell_coord( _dest_cell );
	double dx = dest_pos._x - r_state._x;
	double dy = dest_pos._y - r_state._y;
	double bearing = delta2rad( dx , dy );
	double std_th = rad_hold( r_state._th );	
	return fabs( breaing - std_th );
	
}

// 计算距离：
double 
HybirdAstar::_state_distance( mixstate_t& r_st1 , mixstate_t& r_st2 )
{
	double dx = r_st1._x - r_st2._y;
	double dy = r_st1._y - r_st2._y;
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
		// 存在则不插入：
		if( r_lnpos == r_inpos )
			return ;
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
