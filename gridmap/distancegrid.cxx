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
#include <distancegrid.h>

using namespace std;
using namespace SlamLab;

// -------------------------- 初始化部分 ---------------------------
// 各个值的初始化：
void DistanceMap::_init_map_data( )
{
	// 将所有网格单元均设为最大距离，且关联障碍格坐标为（0，0）
	_max_distance = _grid.width()*_grid.height();
	dm_cell_t dc = { _max_distance , Point2D<size_t>( 0 , 0 ) };
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

