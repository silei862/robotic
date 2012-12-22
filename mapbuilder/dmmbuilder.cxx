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
#include <stdint.h>
#include <cmath>
#include <list>
using namespace std;

#include <debug.h>
#include <dmmbuilder.h>

using namespace SlamLab;

const int DMMBuilder::delta_i[DELTA_NUMS]={ -1,-1,-1, 0,0, 1,1,1 };
const int DMMBuilder::delta_j[DELTA_NUMS]={ -1, 0, 1,-1,1,-1,0,1 };

// 设置地图更新向量的操作符
DMMBuilder&
SlamLab::operator>>( uvectors_t& r_uvec , DMMBuilder& r_dmmbd )
{
	r_dmmbd.set_update_vector( r_uvec );
	return r_dmmbd;
}

// ------------------------------- 更新器初始化 --------------------------------
DMMBuilder::DMMBuilder( int32_t max_val , int32_t th_low , int32_t th_high, double win_size )
{
	_max_val = max_val;
	_th_low = th_low;
	_th_high = th_high;
	_win_size = win_size;
	_robot_pos = float_pos_t( 0.0 , 0.0 );
	_prev_pos = _robot_pos;
	_first_pos_change = true;
}

// ------------------------------ 对外接口 ------------------------------------
// 更新操作：
void DMMBuilder::update()
{
	ASSERT( p_uvecs && p_cmap && p_dmap );
	for( size_t i =0 ; i< p_uvecs->size(); i ++ )
	{
		// 使用更新向量更新概率网格地图
		update_vector_t& r_uvec = (*p_uvecs)[ i ];
		int32_t val = int32_t((*p_cmap)( r_uvec.x , r_uvec.y ));
		int32_t new_val = val + r_uvec.delta;
		// 值限定并更新概率网格地图
		if( new_val > _max_val ) 
			new_val = _max_val;
		if( new_val < 0 )
			new_val = 0;
		(*p_cmap)( r_uvec.x , r_uvec.y ) = uint8_t( new_val );
		// 新计算值阈值判断：
		if( new_val > _th_high && val <=_th_high )
			_add_obstacle( r_uvec.x , r_uvec.y );
		if( new_val < _th_low && val >=_th_low )
			_clear_obstacle( r_uvec.x , r_uvec.y );
	}
	// 进行距离网格地图的更新计算
	_clearobstacle_update();
	_addobstacle_update();
	// 清空更新向量：
	p_uvecs->clear();
}

// 操作符重载：
DMMBuilder&
DMMBuilder::operator()( HIMMGrid& r_camp, double rx , double ry )
{
	_change_pos( rx , ry );
	set_cmap( r_camp );
	return *this;
}
DistanceMap&
DMMBuilder::operator>>( DistanceMap& r_dmap )
{
	set_dmap( r_dmap );
	update();
	return *p_dmap;
}

// 设置关联更新向量组
inline void 
DMMBuilder::set_update_vector( uvectors_t& r_uvecs )
{
	p_uvecs = &r_uvecs;
}
// 设置概率地图
inline void 
DMMBuilder::set_cmap( HIMMGrid& r_cmap )
{
	p_cmap = &r_cmap;
}
// 设置距离地图
inline void
DMMBuilder::set_dmap( DistanceMap& r_dmap )
{
	p_dmap = &r_dmap;
}

// ------------------------------ 内部操作部分 ---------------------------------
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
				it --;
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
void DMMBuilder::_add_obstacle( double x , double y )
{
	grid_pos_t g_pos = p_dmap->pos2sq( x , y );
	add_unit_t au;
	// 设置更新级别
	au._level = 0;
	// 障碍及更新格坐标初始化:
	au._pos = g_pos;
	au._obstacle_pos = g_pos;
	// 更新值及序值计算：
	_fill_unit_update_val( au );
	_fill_unit_order_val( au );
	_insert_addunit( au );
}
// 清除障碍格：
void DMMBuilder::_clear_obstacle( double x , double y )
{
	grid_pos_t g_pos = p_dmap->pos2sq( x , y );
	clr_unit_t cu;
	// 填充解构体：
	cu._pos = g_pos;
	cu._obstacle_pos = g_pos;
	_clr_queue.push_back( cu );
}
// 根据序号计算某个网格周边八个坐标,以活动窗口为范围限定
bool DMMBuilder::_to_around( grid_pos_t& r_pos , size_t idx )
{
	int i = int(r_pos._x) + delta_i[ idx ];
	int j = int(r_pos._y) + delta_j[ idx ];
	// 如果为负，则计算结果无效
	if( i < 0 || j < 0 )
		return false;
	// 如果不在网格地图范围内则结果无效
	if(!_in_win( size_t(i), size_t(j) ) )
		return false;
	// 通过上面检测，结果有效
	r_pos._x = size_t( i );
	r_pos._y = size_t( j );
	return true;
}
// 根据序号计算某网格周边八个邻格，以地图为限定范围
bool DMMBuilder::_to_around_all( grid_pos_t& r_pos , size_t idx )
{
	int i = int(r_pos._x) + delta_i[ idx ];
	int j = int(r_pos._y) + delta_j[ idx ];
	// 如果为负，则计算结果无效
	if( i < 0 || j < 0 )
		return false;
	// 如果不在网格地图范围内则结果无效
	if(!p_dmap->in( size_t(i), size_t(j) ) )
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

// 障碍格清除更新计算：
void DMMBuilder::_clearobstacle_update()
{
	while( !_clr_queue.empty() )
	{
		clr_unit_t& r_cu = _clr_queue.front();
		// 对单元格进行清除操作：
		p_dmap->clear_cell( r_cu._pos );
		// 进行扩散清除：
		for( size_t i =0 ; i < DELTA_NUMS ; i++ )
		{
			// 以已清除单元格坐标为基础
			grid_pos_t ar_pos = r_cu._pos;
			if( _to_around_all( ar_pos , i ) )
			{
				dm_cell_t& r_dcell = ( *p_dmap )( ar_pos );
				std::cout<<"CQ="<<_clr_queue.size()<<" AQ="<<_add_queue.size()<<std::endl;
				// 检查障碍关联性
				if( r_dcell._ob_pos == r_cu._obstacle_pos )
				{
					// 相关则将加入清除更新队列
					clr_unit_t cu;
					cu._pos = ar_pos;
					cu._obstacle_pos = r_cu._obstacle_pos;
					_clr_queue.push_back( cu );
				}
				else // 否则将该格作为更新前沿插入更新队列：
				{
					add_unit_t au;
					au._level = 0;
					// 填充坐标：
					au._obstacle_pos = r_dcell._ob_pos;
					au._pos = ar_pos;
					// 填充更新值及更新序值
					_fill_unit_update_val( au );
					_fill_unit_order_val( au );
					_add_queue.push_front( au );
				}
			}
		}
		_clr_queue.pop_front();
	}
	/*
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
				if( _to_around_all( cell_pos , idx ) )
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
	*/
}

// 窗口越界判别：
bool DMMBuilder::_in_win( size_t i , size_t j )
{
	dm_win_t& r_dmwin = p_dmap->get_win( _robot_pos , _win_size/2 );
	size_t i0 = size_t( r_dmwin.get_x_base() );
	size_t j0 = size_t( r_dmwin.get_y_base() );
	size_t in = i0 + size_t( r_dmwin.cell_cols() );
	size_t jn = j0 + size_t( r_dmwin.cell_rows() );
	return ( i >= i0 )&&(i < in )&&( j >= j0 )&&( j < jn );
}

// 窗口变更计算
void DMMBuilder::_change_pos( double rx , double ry )
{
	if( _first_pos_change )
	{
		// 首次初始化
		_first_pos_change = false;
		_robot_pos._x = rx;
		_robot_pos._y = ry;
		_prev_pos = _robot_pos;
		return;
	}
	// 当前坐标更新
	_robot_pos._x = rx;
	_robot_pos._y = ry;
	// 变化量计算：
	double delta_x = _robot_pos._x - _prev_pos._x ;
	double delta_y = _robot_pos._y - _prev_pos._y;
	size_t delta_i = size_t( fabs( delta_x ) / p_dmap->cell_size() );
	size_t delta_j = size_t( fabs( delta_y ) / p_dmap->cell_size() );
	// 行列序号无变化无需进行变更计算：
	if( delta_i ==0 && delta_j==0 )
	{
		_prev_pos = _robot_pos;
		return;
	}
	// 获取活动窗口参数
	dm_win_t& r_win = p_dmap->get_win( _robot_pos , _win_size/2 );
	size_t i_base = size_t(r_win.get_x_base());
	size_t j_base = size_t(r_win.get_y_base());
	size_t cols = size_t(r_win.cell_cols());
	size_t rows = size_t(r_win.cell_rows());
	// 根据位置变化量选择不同的行列序号范围
	size_t i_min , i_max , j_min , j_max;
	size_t ii , jj;
	if( delta_x <  0 )
	{
		i_min = delta_i + 1;
		i_max = cols - 1;
		ii = delta_i + 1;
	}
	else
	{
		i_min = 0;
		i_max = cols - delta_i - 1;
		ii = cols - delta_i -1;
	}
	if( delta_y < 0 )
	{
		j_min = delta_j + 1;
		j_max = rows - 1;
		jj = delta_j + 1;
	}
	else
	{
		j_min = 0;
		j_max = rows - delta_j - 1;
		jj = rows - delta_j - 1;
	}
	// 将窗口内的行列序号转换为全局行列序号：
	i_min += i_base;
	j_min += j_base;
	i_max += i_base;
	j_min += j_base;
	ii += i_base;
	jj += j_base;
	// 将更新“前沿”插入更新队列
	for( size_t i = i_min ; i<=i_max ; i++ )
	{
		add_unit_t au;
		au._pos = grid_pos_t( i , jj );
		au._obstacle_pos = (*p_dmap)( au._pos )._ob_pos;
		au._level = 0 ;
		_fill_unit_update_val( au );
		_fill_unit_order_val( au );
		_insert_addunit( au );
	}
	for( size_t j = j_min ; j<=j_max ; j++ )
	{
		add_unit_t au;
		au._pos = grid_pos_t( ii , j );
		au._obstacle_pos = (*p_dmap)( au._pos )._ob_pos;
		au._level = 0;
		_fill_unit_update_val( au );
		_fill_unit_order_val( au );
		_insert_addunit( au );
	}

}


