/*
 * =====================================================================================
 *
 *       Filename:  himmbuilder.cxx
 *
 *    Description:  Histogramic in-motion mapping实现
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 22时14分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <exception.h>
#include <debug.h>
#include "himmbuilder.h"

using namespace SlamLab;

const double HIMMBuilder::tpl_val[TPL_NUM]={ 0.5, 0.5, 0.5, \
						   					  0.5, 1.0, 0.5, \
											  0.5, 0.5, 0.5 };
const double HIMMBuilder::max_reading = 5.0;
const double HIMMBuilder::min_reading = 0.0;
HIMMBuilder::HIMMBuilder( uint8_t inc , 
			uint8_t dec , 
			uint8_t max )
		:_tpl(DEF_TPLSIZE,DEF_TPLSIZE),
		_inc(inc),_dec(dec),_max(max),
		p_pos2d( NULL ),
		p_ranger( NULL ),
		p_map( NULL )
{
	// 初始化模板数据
	for( uint32_t y=0;y<DEF_TPLSIZE;y++ )
		for( uint32_t x=0;x<DEF_TPLSIZE;x++ )
			_tpl[y][x] = tpl_val[y*DEF_TPLSIZE+x];
}

void HIMMBuilder::build()
{
	ASSERT( p_pos2d && p_ranger && p_map );
	// 获取机器人中心坐标
	double robot_x = p_pos2d->get_x_pos();
	double robot_y = p_pos2d->get_y_pos();
	double robot_yaw = p_pos2d->get_yaw();
	// 依次计算各个数据
	for( size_t i = 0 ; i< p_ranger->ranger_num(); i++ )
	{
		// 获取传感器数据
	    double rg_reading = (*p_ranger)[i];
		// 获取传感器安装姿态，作为修正
		Pose3D& ranger_pose = p_ranger->get_pose( i );
		Point3D<double> rg_pos = ranger_pose._pos;
		// 计算传感器全局坐标：
		double rg_x = robot_x + rg_pos._x*cos( robot_yaw ) - rg_pos._y*sin( robot_yaw );
		double rg_y = robot_y + rg_pos._x*sin( robot_yaw ) + rg_pos._y*cos( robot_yaw );
		//计算障碍点的坐标：
		double ob_x = rg_x + rg_reading*cos( ranger_pose._rz + robot_yaw );
		double ob_y = rg_y + rg_reading*sin( ranger_pose._rz + robot_yaw );
		// 更新地图,只有在传感器读数处于范围内方才进行增量计算：
		if( rg_reading < max_reading )	
			cell_inc( ob_x , ob_y );
		cell_dec( robot_x , robot_y , ob_x , ob_y );
	}
}

HIMMGrid& HIMMBuilder::operator>>( HIMMGrid& r_hg )
{
	attach_map( r_hg );
	build();
	return r_hg;
}

void HIMMBuilder::cell_inc( double x , double y )
{
	if( !p_map->in( x ,y ) )
		return ;
	int cell_val = int((*p_map)( x, y));
	// 生成模板坐标索引：
	for( int p = -1 ; p <=1 ; p++ )
		for( int q = -1 ; q <=1 ; q++ )
		{
			double cx = x + p_map->cell_size()*double(q);
			double cy = y + p_map->cell_size()*double(p);
			if( p_map->in( cx ,cy ) )
			{
				uint8_t nc_val = (*p_map)(cx,cy);
				cell_val+=int(double(nc_val)*_tpl[p+1][q+1]);
			}
		}
	cell_val+=_inc;
	// 限制上限值：
	if( cell_val > _max )
		cell_val = _max;
	// 回写：
	(*p_map)(x,y) = uint8_t(cell_val);
}

void HIMMBuilder::cell_dec( double x0,double y0,double x1,double y1 )
{
	double delta_x = x1 - x0 ;
	double delta_y = y1 - y0 ;
	double abs_dx = fabs( delta_x );
	double abs_dy = fabs( delta_y );
	double cell_size = p_map->cell_size();
	double inc_x = cell_size;
	double inc_y = cell_size;
	int inc_num;

	if( abs_dx < cell_size && abs_dy < cell_size )
		return;

	if( abs_dx > abs_dy )
	{
		inc_y *= delta_y/abs_dx;
		inc_x *= delta_x/abs_dx;
		inc_num = abs_dx/cell_size;
	}
	else
	{
		inc_x *= delta_x/abs_dy;
		inc_y *= delta_y/abs_dy;
		inc_num = abs_dy/cell_size;
	}	
	
	double bx = x0;
	double by = y0;
	for( int i = 0; i<inc_num; i++)
	{
		bx +=inc_x;
		by +=inc_y;
		if( !p_map->in( bx,by ) )
			continue;
		int cell_val =int((*p_map)( bx , by ));
		cell_val -=_dec;
		if( cell_val < 0 )
			cell_val =0;
		(*p_map)( bx , by ) = cell_val; 
	}
}

SlamLab::HIMMBuilder& SlamLab::operator>>( SlamLab::RangerBridge& r_rb , SlamLab::HIMMBuilder& r_builder )
{
	r_builder.attach_ranger( r_rb );
	return r_builder;
}
