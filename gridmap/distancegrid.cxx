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
#include <cmath>
#include <distancegrid.h>

using namespace std;
using namespace SlamLab;

// -------------------------- 初始化部分 ---------------------------
// 各个值的初始化：
void DistanceMap::_init_map_data( )
{
	// 将所有网格单元均设为最大距离，且关联障碍格坐标为（0，0）
	double w = _grid.width();
	double h = _grid.height();
	_max_distance = sqrt( w*w + h*h );
	dm_cell_t dc = { _max_distance , grid_pos_t( 0 , 0 ) };
	_grid.set_all_val( dc );
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

DistanceMap::DistanceMap( size_t cols , size_t rows , double cell_size , float_pos_t org )
	:_grid( cols , rows , cell_size , org )
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
}

// --------------------- 对外接口----------------------
// 设置基本属性：
void DistanceMap::set_attr( size_t cols , size_t rows , double cell_size , float_pos_t org )
{
	_grid.set_attr( cols , rows , cell_size , org );
}

// 获取距离网格各类基本属性
size_t DistanceMap::cols()
{
	return size_t(_grid.cell_cols());
}

size_t DistanceMap::rows()
{
	return size_t(_grid.cell_rows());
}

double DistanceMap::width()
{
	return _grid.width();
}

double DistanceMap::height()
{
	return _grid.height();
}

double DistanceMap::cell_size()
{
	return _grid.cell_size();
}

float_pos_t DistanceMap::origin()
{
	return _grid.get_origin();
}

// 获取距离网格引用
dm_data_t& DistanceMap::get_dm_grid()
{
	return _grid;
}
// 获取距离网格的窗口
dm_win_t& DistanceMap::get_win( float_pos_t org , double width , double height )
{
	return _grid.get_win( org._x , org._y , width , height );
}

dm_win_t& DistanceMap::get_win( float_pos_t org , double radius )
{
	return _grid.get_win( org._x , org._y , radius );
}
// 坐标转换为行列序号：
grid_pos_t DistanceMap::pos2sq( float_pos_t pos )
{
	grid_pos_t sq;
	Point2D<uint32_t> usq = _grid.pos2sq( pos );
	sq._x = size_t( usq._x );
	sq._y = size_t( usq._y );
	return sq;
}

grid_pos_t DistanceMap::pos2sq( double x , double y )
{
	grid_pos_t sq;
	Point2D<uint32_t> usq = _grid.pos2sq( x , y );
	sq._x = size_t( usq._x );
	sq._y = size_t( usq._y );
	return sq;
}
// 行列序号转换为坐标：
float_pos_t DistanceMap::cell_coord( size_t i , size_t j )
{
	return _grid.cell_coord( uint32_t(i) , uint32_t(j) );
}

float_pos_t DistanceMap::cell_coord( grid_pos_t& pos )
{
	return _grid.cell_coord( pos._x , pos._y );
}

// 越界判断
bool DistanceMap::in( double x , double y )
{
	return _grid.in( x, y);
}

bool DistanceMap::in( size_t i , size_t j )
{
	return ( i < _grid.cell_cols() )&&( j < _grid.cell_rows() ); 
}

// 各种存取接口：
dm_cell_t&
DistanceMap::operator()( size_t i , size_t j )
{
	return _grid( uint32_t(i) ,uint32_t(j) );
}

dm_cell_t&
DistanceMap::operator()( grid_pos_t& pos )
{
	return _grid( uint32_t(pos._x) , uint32_t(pos._y) );
}

dm_cell_t&
DistanceMap::operator()( double x , double y )
{
	return _grid( x , y );
}

dm_cell_t&
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

void
DistanceMap::clear_cell( grid_pos_t& r_pos )
{
	this->clear_cell( r_pos._x , r_pos._y );
}

void
DistanceMap::clear_cell( float_pos_t& r_pos )
{
	this->clear_cell( r_pos._x , r_pos._y );
}

double DistanceMap::max_distance()
{
	return _max_distance;
}

std::ostream& std::operator<<( std::ostream& r_os , SlamLab::DistanceMap& r_dm )
{
	double w = r_dm.width();
	double h = r_dm.height();
	double max_val = sqrt( w*w + h*h );
	for( size_t j = r_dm.rows() ; j > 0 ; j-- )
	{
		for( size_t i = 0 ; i < r_dm.cols() ; i++ )
		{
					if( 0==i || (r_dm.cols()-1) == i )
			{
				r_os << "| ";
				continue;
			}
			if( 1==j || r_dm.rows() == j )
			{
				r_os << "__";
				continue;
			}

			double d = r_dm( i , j-1 )._d;
			int pv = d/max_val*99;
			if( pv < 10 )
				r_os <<" "<<pv;
			else
				r_os <<pv;
		}
		r_os<<std::endl;
	}
	return r_os;
}

