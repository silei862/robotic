/*
 * =====================================================================================
 *
 *       Filename:  distancegrid.cxx
 *
 *    Description:  距离网格地图实现文件
 *
 *        Version:  1.0
 *        Created:  2012年12月17日 15时05分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdint.h>
#include <distancegrid.h>

using namespace std;
using namespace SlamLab;

// -------------------------- 初始化部分 ---------------------------
// 各个值的初始化：
void DistanceMap::_init_map_data( )
{
	// 将所有网格单元均设为最大距离，且关联障碍格坐标为（0，0）
	_max_distance = _grid.width()*_grid.height();
	dm_cell_t dc = { _max_distance , grid_pos_t( 0 , 0 ) };
	_grid.set_all_val( dc );
	_queue_add.clear();
	_queue_clr.clear();
}

DistanceMap::DistanceMap( )
{
	_init_map_data();
}

DistanceMap::DistanceMap( double width , double height , double cell_size, Point2D<double> org )
	:_grid( width ,height , cell_size , org )
{
	_init_map_data();
}

// 参照概率网格地图参数，构建距离网格地图
DistanceMap::DistanceMap( HIMMGrid& r_hg )
	:_grid( r_hg.width(), 
			r_hg.height(),
			r_hg.cell_size(),
			r_hg.get_origin() )
{
	_init_map_data();
}

DistanceMap::DistanceMap( const DistanceMap& dm )
{
	_grid = dm._grid;
	_queue_add.clear();
	_queue_clr.clear();
}

// --------------------- 对外接口----------------------
// 获取距离网格各类基本属性
inline size_t DistanceMap::cols()
{
	return size_t(_grid.cell_cols());
}

inline size_t DistanceMap::rows()
{
	return size_t(_grid.cell_rows());
}

inline double DistanceMap::width()
{
	return _grid.width();
}

inline double DistanceMap::height()
{
	return _grid.height();
}

inline double DistanceMap::cell_size()
{
	return _grid.cell_size();
}

inline float_pos_t DistanceMap::origin()
{
	return _grid.get_origin();
}

// 获取距离网格引用
inline dm_data_t& DistanceMap::get_dm_grid()
{
	return _grid;
}
// 获取距离网格的窗口
inline dm_win_t& DistanceMap::get_win( float_pos_t org , double width , double height )
{
	return _grid.get_win( org._x , org._y , width , height );
}

inline dm_win_t& DistanceMap::get_win( float_pos_t org , double radius )
{
	return _grid.get_win( org._x , org._y , radius );
}
// 坐标转换为行列序号：
inline grid_pos_t DistanceMap::pos2sq( float_pos_t pos )
{
	grid_pos_t sq;
	Point2D<uint32_t> usq = _grid.pos2sq( pos );
	sq._x = size_t( usq._x );
	sq._y = size_t( usq._y );
	return sq;
}

inline grid_pos_t DistanceMap::pos2sq( double x , double y )
{
	grid_pos_t sq;
	Point2D<uint32_t> usq = _grid.pos2sq( x , y );
	sq._x = size_t( usq._x );
	sq._y = size_t( usq._y );
	return sq;
}
// 越界判断
inline bool DistanceMap::in( double x , double y )
{
	return _grid.in( x, y);
}

inline bool DistanceMap::in( size_t i , size_t j )
{
	return _grid.in( uint32_t( i ), uint32_t( j ) );
}

// 各种存取接口：
inline dm_cell_t&
DistanceMap::operator()( size_t i , size_t j )
{
	return _grid( uint32_t(i) ,uint32_t(j) );
}

inline dm_cell_t&
DistanceMap::operator()( grid_pos_t& pos )
{
	return _grid( uint32_t(pos._x) , uint32_t(pos._y) );
}

inline dm_cell_t&
DistanceMap::operator()( double x , double y )
{
	return _grid( x , y );
}

inline dm_cell_t&
DistanceMap::operator()( float_pos_t& pos )
{
	return _grid( pos._x , pos._y );
}

// 障碍格清除，恢复初始状态
void DistanceMap::clear_cell( size_t i , size_t j )
{
	dm_cell_t& r_dcell = (*this)( i , j );
	r_dcell._d = _max_distance;
	r_dcell._ob_pos = grid_pos_t( 0 , 0 );
}

void DistanceMap::clear_cell( double x , double y )
{
	dm_cell_t& r_dcell = (*this)( x ,y );
	r_dcell._d = _max_distance;
	r_dcell._ob_pos = grid_pos_t( 0 , 0 );
}

inline void
DistanceMap::clear_cell( grid_pos_t& r_pos )
{
	this->clear_cell( r_pos._x , r_pos._y );
}

inline void
DistanceMap::clear_cell( float_pos_t& r_pos )
{
	this->clear_cell( r_pos._x , r_pos._y );
}


