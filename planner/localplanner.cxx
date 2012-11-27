/*
 * =====================================================================================
 *
 *       Filename:  localplanner.cxx
 *
 *    Description:  短距离路径规划器
 *
 *        Version:  1.0
 *        Created:  2012年11月27日 11时13分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <cmath>
#include <stdlib.h>
#include "localplanner.h"

bool SlamLab::goto_pos( Position2DBridge& r_posbr , const Position2D& dest_pos , SteerCtrl& r_stc , double err)
{
	// 计算与目标方向距离：
	double delta_x = dest_pos._x - r_posbr.get_x_pos();
	double delta_y = dest_pos._y - r_posbr.get_y_pos();
	double distance = sqrt( delta_x*delta_x + delta_y*delta_y );
	// 判断是否到达目的地:
	if( distance < err )
	{
		r_stc._ahead_veloc = 0.0;
		r_stc._angular_veloc = 0.0;
		return true;
	}
	
	double bearing = acos(delta_x/distance);
	if( delta_y < 0 )
		bearing = -1.0*bearing;
	double delta_beta = bearing - r_posbr.get_yaw();
	// 规整到180度范围内
	if( delta_beta > PI )
		delta_beta -= 2*PI;
	if( delta_beta < -1*PI )
		delta_beta += 2*PI;
	// 计算角速度产生速度控制
	r_stc._angular_veloc = delta_beta/3;
	if( fabs(r_stc._angular_veloc) > 0.05 )
		r_stc._ahead_veloc = 0.2;
	else
		r_stc._ahead_veloc = 0.6;
	return false;
}


// ------------------------- simple collision avoid ---------------------------
static const double safe_distance = 0.5;
static const double default_speed = 0.7;
static const double default_turnrate = 1.7;

bool SlamLab::simple_collision_avoid( RangerBridge& r_rb , SteerCtrl& r_stc )
{
	bool ret_val = false;
	double speed = default_speed;
	double turnrate = 0.0;
	int max_range_num = r_rb.ranger_num();
	if(!max_range_num )
		return false;
	double min_index =0;
	double min_val = r_rb[min_index];
	for( int i =1; i<max_range_num; i++ )
		if( min_val > r_rb[i] )
		{
			min_index = i;
			min_val = r_rb[i];
		}
	if( min_val < safe_distance )
	{
		Pose3D& r_pose = r_rb.get_pose(min_index);
		speed = 0.0;
		/* 	if( r_pose._rz >( PI/(-4)) && r_pose._rz<(PI/4) )
			turnrate= default_turnrate;
		else
			turnrate= r_pose._rz/(-4); */
		if( r_pose._rz >0 )
			turnrate = -1*default_turnrate;
		else
			turnrate = default_turnrate;
		ret_val = true;
	}
	r_stc._ahead_veloc = speed;
	r_stc._angular_veloc = turnrate;
	return ret_val;
}

