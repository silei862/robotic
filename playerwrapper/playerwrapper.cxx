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
#include <cmath>
#include <debug.h>
#include <misc.h>
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
	r_ranger.GetIndex();
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

// -------------------- 带正太分布噪声的距离传感器包装类-------------------
double RangerNoisePlayer::get_data( size_t index )
{
	r_ranger.GetIndex();
	double d = r_ranger[ index ];
	double noise = randnormal( 0.0 , d*_portion );
	double nd = d+noise;
	if( nd <0 )
		return 0;
	else
		return nd;
}

size_t RangerNoisePlayer::ranger_num()
{
	return r_ranger.GetRangeCount();
}

Pose3D& RangerNoisePlayer::get_pose( size_t index )
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

// ---------------------------- 带噪声的位置机构 -----------------------
void Position2DNoisePlayer::set_speed( double a_speed , double yaw_speed )
{
	double aspeed_noise;
	// 偏差范围限定：
	//do
	//{
		aspeed_noise = randnormal( 0.0 , _sigma_aspeed );
	//}while( fabs( aspeed_noise ) < _sigma_aspeed*2 );

	double yspeed_noise;
	//do
	//{
		yspeed_noise = randnormal( 0.0 , _sigma_yspeed );
	//}while( fabs( yspeed_noise ) < _sigma_yspeed*2 );

	// 噪声加入控制速度角速度
	double aspeed = a_speed + aspeed_noise;
	double yspeed = yaw_speed + yspeed_noise;
	r_pos2d.SetSpeed( aspeed , yspeed );
}

double Position2DNoisePlayer::get_x_pos()
{
	double noise;
	// 范围限定：
	//do
	//{
		noise = randnormal( 0.0 , _sigma_xy );
	//}while( fabs( noise ) < _sigma_xy*2 );
	return r_pos2d.GetXPos() + noise ;
}

double Position2DNoisePlayer::get_y_pos()
{
	double noise;
	// 范围限定：
	//do
	//{
		noise = randnormal( 0.0 , _sigma_xy );
	//}while( fabs( noise ) < _sigma_xy*2 );
	return r_pos2d.GetYPos() + noise;
}

double Position2DNoisePlayer::get_yaw()
{
	double noise;
	//do
	//{
		noise = randnormal( 0.0 , _sigma_yaw );
	//}while( fabs( noise ) < _sigma_yaw*2 );
	return r_pos2d.GetYaw() + noise;
}
