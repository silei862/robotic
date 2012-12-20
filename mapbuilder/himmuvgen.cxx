/*
 * =====================================================================================
 *
 *       Filename:  himmuvgen.cxx
 *
 *    Description:  更新向量生成器实现
 *
 *        Version:  1.0
 *        Created:  2012年12月14日 10时05分41秒
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
#include "himmuvgen.h"

using namespace SlamLab;

// ----------------------- 辅助操作符 -----------------------------
HIMMUVGen& 
SlamLab::operator>>( RangerBridge& r_ranger , HIMMUVGen& r_uvg )
{
	r_uvg.set_ranger( r_ranger );
	return r_uvg;
}

// ----------------------- 常量 -----------------------------------
const double HIMMUVGen::tpl_val[ TPL_NUM ] = { 0.5, 0.5, 0.5, \
											   0.5, 1.0, 0.5, \
											   0.5, 0.5, 0.5 };	   
const double HIMMUVGen::def_max_reading = 5.0;
const double HIMMUVGen::def_min_reading = 0.0;

// ----------------------- 对外接口 --------------------------------
HIMMUVGen::HIMMUVGen( int inc , int dec , double max_reading )
	:_inc( inc ) , _dec( dec ), _max_reading( max_reading ),
	_tpl( DEF_TPL_SIZE ,DEF_TPL_SIZE )
{
	// 更新模板的初始化，装载预设定数组
	for( int y = 0; y < DEF_TPL_SIZE; y++ )
		for( int x =0; x < DEF_TPL_SIZE; x++ )
			_tpl[y][x] = tpl_val[ y*DEF_TPL_SIZE+x ];
	// 将关联类型指针初始化为空
	p_pos2d = NULL;
	p_ranger = NULL;
	p_uvectors = NULL;
	p_himmgrid = NULL;
}

HIMMUVGen& 
HIMMUVGen::operator()( Position2DBridge& r_pos2d , HIMMGrid& r_hg )
{
	set_pos2d( r_pos2d );
	set_himmgrid( r_hg );
	return *this;
}

uvectors_t& 
HIMMUVGen::operator>>( uvectors_t& r_uv )
{
	set_uvector( r_uv );
	ASSERT( p_pos2d && p_ranger && p_uvectors );
	generator();
	return *p_uvectors;
}

// ----------------------- 内部接口 -------------------------------
inline void 
HIMMUVGen::set_pos2d( Position2DBridge& r_pos2d )
{
	p_pos2d = &r_pos2d;
}

inline void 
HIMMUVGen::set_ranger( RangerBridge& r_ranger )
{
	p_ranger = &r_ranger;
}

inline void 
HIMMUVGen::set_uvector( uvectors_t& r_uvs )
{
	p_uvectors = &r_uvs;
}

inline void 
HIMMUVGen::set_himmgrid( HIMMGrid& r_hg )
{
	p_himmgrid = &r_hg;
}

// 更新向量生成器
void HIMMUVGen::generator()
{
	// 获取移动机器人状态
	double rb_x = p_pos2d->get_x_pos();
	double rb_y = p_pos2d->get_y_pos();
	double rb_yaw = p_pos2d->get_yaw();
	// 逐个获取数据并进行计算
	for( int i= 0; i < p_ranger->ranger_num(); i++ )
	{
		// 从传感器桥接器获取传感器各个参数：
		double rg_reading = ( *p_ranger )[ i ];
		Pose3D& rg_pose = p_ranger->get_pose( i );
		Point3D<double> rg_pos = rg_pose._pos;
		// 计算传感器全局坐标
		double rg_x = rb_x + rg_pos._x*cos( rb_yaw ) - rg_pos._y*sin( rb_yaw );
		double rg_y = rb_y + rg_pos._x*sin( rb_yaw ) + rg_pos._y*cos( rb_yaw );
		// 计算障碍点的坐标
		double ob_x = rg_x + rg_reading*cos( rb_yaw + rg_pose._rz );
		double ob_y = rg_y + rg_reading*sin( rb_yaw + rg_pose._rz );
		// 进行更新计算：
		if( rg_reading < _max_reading )
			inc_vec_gen( ob_x , ob_y );
		dec_vec_gen( rg_x, rg_y , ob_x , ob_y );
	}
}

void HIMMUVGen::inc_vec_gen( double x , double y )
{
	// 根据模板计算障碍处更新变化量：
	double delta =0.0;
	for( int q = -1 ; q <= 1 ; q ++ )
		for( int p = -1 ; p <=1 ; p++ )
		{
			// 计算3X3范围的单元坐标：
			double cell_x = x + p_himmgrid->cell_size()*double(p);
			double cell_y = y + p_himmgrid->cell_size()*double(q);
			// 边界检查,在网格内则累加到变化量上
			if( p_himmgrid->in( cell_x ,cell_y ) )
				delta += _tpl[q+1][p+1]*(*p_himmgrid)( cell_x , cell_y );	
		}
	// 最后再加上更新值：
	delta += _inc;
	// 生成更新单元并加入更新向量组
	update_vector_t uv = { x , y , delta };
	p_uvectors->push_back( uv );
	
}

void HIMMUVGen::dec_vec_gen( double x0, double y0, double x1, double y1 )
{
	// 计算两个坐标之间的差值及对应绝对值
	double delta_x = x1 - x0;
	double delta_y = y1 - y0;
	double abs_dx = fabs( delta_x );
	double abs_dy = fabs( delta_y );
	// 获取单元格边长
	double cell_size = p_himmgrid->cell_size();
	// 步数
	size_t step_num;
	// 如果变化量不足一个单元格，则不处理
	if( abs_dx < cell_size && abs_dy < cell_size )
		return;
	// 变化量大的作为步数的计数基准
	if( abs_dx > abs_dy )
		step_num = size_t( abs_dx/cell_size );
	else
		step_num = size_t( abs_dy/cell_size );
	// 分别计算步长：
	double step_x = delta_x/step_num;
	double step_y = delta_y/step_num;
	// 初始化步长坐标：
	double xx = x0;
	double yy = y0;
	// 生成x0,y0到x1,y1连线间的网格坐标,最后一格不做减量
	for( size_t i = 0 ; i < step_num-1 ; i++ )
	{
		// 生成更新向量并加入更新向量组：
		update_vector_t uv = { xx , yy , -1*_dec };
		p_uvectors->push_back( uv );
		// 计算下一步的坐标
		xx += step_x;
		yy += step_y;
	}
}
