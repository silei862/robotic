/*
 * =====================================================================================
 *
 *       Filename:  dmmbuilder.cxx
 *
 *    Description:  双地图更新类
 *
 *        Version:  1.0
 *        Created:  2012年12月17日 19时21分15秒
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
#include <list>
using namespace std;

#include <debug.h>
#include <dmmbuilder.h>

using namespace SlamLab;

const int DMMBuilder::delta_i[DELTA_NUMS]={ -1,-1,-1, 0,0, 1,1,1 };
const int DMMBuilder::delta_j[DELTA_NUMS]={ -1, 0, 1,-1,1,-1,0,1 };

// ------------------------------- 更新器初始化 --------------------------------
DMMBuilder::DMMBuilder()
	:p_uvec( NULL ) , p_cmap( NULL ) , p_dmap( NULL )
{

}

DMMBuilder::DMMBuilder( uvectors_t& r_uvec , HIMMGrid& r_cmap , DistanceMap& r_dmap )
	:p_uvec( &r_uvec ) , p_cmap( &r_cmap ) , p_dmap( &r_dmap )
{

}
// ------------------------------ 对外接口 ------------------------------------
// 操作符重载：
DMMBuilder& DMMBuilder::operator()( HIMMGrid& r_camp )
{
	_set_cmap( r_camp );
	return *this;
}

HIMMGrid& DMMBuilder::operator>>( HIMMGrid& r_cmap )
{

}

// ------------------------------ 内部操作部分 ---------------------------------
// 设置关联更新向量组
void DMMBuilder::_set_update_vector( uvectors_t& r_uvec )
{
	p_uvec = &r_uvec;
}
// 设置概率地图
void DMMBuilder::_set_cmap( HIMMGrid& r_cmap )
{
	p_cmap = &r_cmap;
}
// 设置距离地图
void DMMBuilder::_set_dmap( DistanceMap& r_dmap )
{
	p_dmap = &r_dmap;
}

// 计算更新单元更新值
void DMMBuilder::_fill_unit_update_val( add_unit_t& r_au )
{
	ASSERT( p_cmap!=NULL  );
	float_pos_t pos = p_cmap->cell_coord( r_au._pos._x , r_au._pos._y );
	float_pos_t ob_pos = p_cmap->cell_coord( r_au._obstacle_pos._x , r_au._obstacle_pos._y );
	// 计算距离值
	double dx = pos._x - ob_pos._x;
	double dy = pos._y - ob_pos._y;
	r_au._update_val = sqrt( dx*dx + dy*dy );
}

// 障碍增加队列单元序值
void DMMBuilder::_fill_unit_order_val( add_unit_t& r_au )
{
	ASSERT( p_dmap!=NULL )
	size_t gw = p_dmap->rows();
	size_t gh = p_dmap->cols();
	// 计算序值
	r_au._order_val = r_au._level*gw*gh + gw*r_au._pos._y + r_au._pos._x;
}

// 更新单元插入队列：
void DMMBuilder::_insert_addunit( add_unit_t& r_au )
{
	// 如果增加更新队列空，则放入后直接返回：
	if( _add_queue.empty() )
	{
		_add_queue.push_back( r_au );
		return;
	}
	// 将更新单元放入合适位置：
	list< add_unit_t >::iterator it= _add_queue.begin();
	for( ;it != _add_queue.end();it++ )
		// 如果序值相等说明更新的是同一个网格：
		if( r_au._order_val == it->_order_val )
		{
			// 比较更新值，如果小于则替换，否则不插入单元
			if( r_au._update_val < it->_update_val )
			{
				_add_queue.insert( it , r_au );
				it = _add_queue.erase( it );
			}
			return;
		}
		// 否则将更新单元插入合适位置：
		else if( r_au._order_val < it->_order_val )
		{
			_add_queue.insert( it , r_au );
			return;
		}
	// 完成循环找不到合适的位置，则加入尾部
	_add_queue.push_back( r_au );
	return;
}

// 增加障碍格：
void DMMBuilder::_add_obstacle( float_pos_t& r_pos )
{
	grid_pos_t g_pos = p_dmap->pos2sq( r_pos );
	add_unit_t au;
	// 设置更新级别
	au._level = 0;
	// 障碍及更新格坐标初始化:
	au._pos = g_pos;
	au._obstacle_pos = g_pos;
	// 更新值及序值计算：
	_fill_unit_update_val( r_au );
	_fill_unit_order_val( r_au );
	_insert_addunit( add_uinit_t& r_au );
}
// 清除障碍格：
void DMMBuilder::_clear_obstacle( float_pos_t& r_pos )
{
	grid_pos_t g_pos = p_dmap->pos2sq( r_pos );
	clr_unit_t cu;
	// 填充解构体：
	cu._pos = g_pos;
	cu._obstacle_pos = g_pos;
	_clr_queue.push_back( cu );
}
// 根据序号计算某个网格周边八个坐标
bool DMMBuilder::_to_around( grid_pos_t& r_pos , size_t idx )
{
	int i = int(r_pos._x) + delta_i[ idx ];
	int j = int(r_pos._y) + delta_j[ idx ];
	// 如果为负，则计算结果无效
	if( i < 0 || j < 0 )
		return false;
	// 如果不再网格地图范围内则结果无效
	if( !p_dmap->in( size_t( i ), size_t( j ) ) )
		return false;
	// 通过上面检测，结果有效
	r_pos._x = size_t( i );
	r_pos._y = size_t( j );
	return true;
}

// 障碍增加更新计算：
void DMMBuilder::_addobstacle_update()
{
	while( !_add_queue.empty() )
	{
		// 取出队列首元素引用：
		add_unit_t& r_au = _add_queue.front();
		// 获取将被距离网格单元：
		dm_cell_t& r_dc = (*p_dmap)( r_au._pos );
		// 若被更新的目标大于更新值，则更新并进行扩散更新
		if( r_dc._d > r_au._update_val )
		{
			r_dc._ob_pos = r_au._obstacle_pos;
			r_dc._d = r_au._update_val;
			// 扩散更新
			for( size_t idx = 0; idx < DELTA_NUMS ; idx++ )
			{
				// 取更新单元中更新位置做基
				grid_pos_t cell_pos = r_au._pos;
				// 坐标有效则可以进行扩散计算：
				if( _to_around( cell_pos , idx ) )
				{
					// 根据cell_pos生成更新单元
					add_unit_t au;
					au._pos = cell_pos;
					au._obstacle_pos = r_au._obstacle_pos;
					// 更新层级加一
					au._level = r_au._level+1;
					// 填充更新值和序值
					_fill_unit_update_val( au );
					_fill_unit_order_val( au );
					// 插入更新队列
					_insert_addunit( au );
				}
			}
		}	
		// 将首元素删除
		_add_queue.pop_front();
	}
}
