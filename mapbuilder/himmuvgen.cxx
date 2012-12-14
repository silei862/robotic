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

// ----------------------- 常量 -----------------------------------
const double HIMMUVGen::tpl_val[ TPL_NUM ] = { 0.5, 0.5, 0.5, \
											   0.5, 1.0, 0.5, \
											   0.5, 0.5, 0.5 };	   
const double HIMMUVGen::def_max_reading = 5.0;
const double HIMMUVGen::def_min_reading = 0.0;

// ----------------------- 对外接口 --------------------------------
HIMMUVGen::HIMMUVGen( double c_sz, int inc , int dec , double max_reading )
	:_cell_size( c_sz ),_inc( inc ) , _dec( dec ), _max_reading( max_reading )
{
	// 更新模板的初始化，装载预设定数组
	for( int y = 0; y < DEF_TPL_SIZE; y++ )
		for( int x =0; x < DEF_TPL_SIZE; x++ )
			_tpl[y][x] = tpl_val[ y*DEF_TPL_SIZE+x ];
}

HIMMUVGen& HIMMUVGen::operator()( Position2DBridge& r_pos2d )
{
	set_pos2d( r_pos2d );
	return *this;
}

uvectors_t& HIMMUVGen::operator>>( uvectors_t& r_uv )
{
	set_uvector( r_uv );
	ASSERT( p_pos2d && p_ranger && p_uvectors );
	generator();
	return *p_uvectors;
}

// ----------------------- 内部接口 -------------------------------
void HIMMUVGen::set_pos2d( Position2DBridge& r_pos2d )
{
	p_pos2d = &r_pos2d;
}

void HIMMUVGen::set_ranger( RangerBridge& r_ranger )
{
	p_ranger = &r_ranger;
}

void HIMMUVGen::set_uvector( uvectors_t& r_uvs )
{
	p_uvectors = r_uvs;
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
		// 清空上次计算结果：
		p_uvectors->clear();
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

		}
}

void HIMMUVGen::dec_vec_gen( double x0, double y0, double x1, double y1 )
{

}
