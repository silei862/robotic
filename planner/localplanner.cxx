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
#include <stdlib.h>
#include "localplanner.h"

static const double safe_distance = 0.5;
static const double default_speed = 0.7;
static const double default_turnrate = 0.7;

void SlamLab::simple_collision_avoid( Position2DBridge& r_pb, RangerBridge& r_rb )
{
	double speed = default_speed;
	double turnrate = 0.0;
	int max_range_num = r_rb.ranger_num();
	if(!max_range_num )
		return;
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
		if( r_pose._rz >( PI/(-4)) && r_pose._rz<(PI/4) )
			turnrate= default_turnrate;
		else
			turnrate= r_pose._rz/(-4);
	}
	r_pb.set_speed( speed , turnrate );
}

