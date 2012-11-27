/*
 * =====================================================================================
 *
 *       Filename:  playerwrapper.cc
 *
 *    Description:  player平台包装实现文件
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 10时50分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <debug.h>
#include "playerwrapper.h"

using namespace SlamLab;
using namespace PlayerCc;

// ---------------------- player sonar wrapper ----------------------
double SonarPlayer::get_data( size_t index )
{
	return r_sonar[index];
}

size_t SonarPlayer::ranger_num()
{
	return r_sonar.GetCount();
}

Pose3D& SonarPlayer::get_pose( size_t index )
{
	player_pose3d_t pp=r_sonar.GetPose( index );
	_pose=Pose3D(Point3D<double>(pp.px,pp.py,pp.pz),pp.proll,pp.ppitch,pp.pyaw);
	return _pose;
}

// ---------------------- player ranger wrapper ---------------------
double RangerPlayer::get_data( size_t index )
{
	//r_ranger.GetIndex();
	return r_ranger[index];
}

size_t RangerPlayer::ranger_num()
{
	return r_ranger.GetRangeCount();
}

Pose3D& RangerPlayer::get_pose( size_t index )
{
	ASSERT( index<r_ranger.GetRangeCount() );
	player_pose3d_t pp=r_ranger.GetElementPose( index );
	_pose=Pose3D(Point3D<double>(pp.px,pp.py,pp.pz),pp.proll,pp.ppitch,pp.pyaw);
	return _pose;
}

// --------------------- player position2D wrapper ------------------------
void Position2DPlayer::set_speed( double a_speed , double yaw_speed )
{
	r_pos2d.SetSpeed( a_speed , yaw_speed );
}

double Position2DPlayer::get_x_pos()
{
	return r_pos2d.GetXPos();
}

double Position2DPlayer::get_y_pos()
{
	return r_pos2d.GetYPos();
}

double Position2DPlayer::get_yaw()
{
	return r_pos2d.GetYaw();
}
