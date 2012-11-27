/*
 * =====================================================================================
 *
 *       Filename:  bridge.cxx
 *
 *    Description:  桥接类的实现文件
 *
 *        Version:  1.0
 *        Created:  2012年11月18日 16时20分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include "misc.h"
#include "bridge.h"
using namespace SlamLab;

// 一些有用的宏
#define GET_WRAPPER( itf ) dynamic_cast< itf >( get_wrapper() )
#define DEF_WRAPPER( p_wp, itf ) itf p_wp=GET_WRAPPER( itf )

// ------------------ RangerBridge ------------------------------------
Pose3D& RangerBridge::get_pose( size_t index )
{
	DEF_WRAPPER( p_rg,IRangerWrapper* );
	return p_rg->get_pose( index );
}

size_t RangerBridge::ranger_num( )
{
	DEF_WRAPPER( p_rg, IRangerWrapper* );
	return p_rg->ranger_num();
}

double RangerBridge::operator[]( size_t index )
{
	DEF_WRAPPER( p_rg, IRangerWrapper* );
	return p_rg->get_data( index );
}

// ------------------- Pose2DBridge -----------------------------------
void Position2DBridge::set_speed( double a_speed , double yaw_speed )
{
	DEF_WRAPPER( p_pw , IPosition2DWrapper* );
	p_pw->set_speed( a_speed ,yaw_speed );
}
double Position2DBridge::get_x_pos( )
{
	DEF_WRAPPER( p_pw , IPosition2DWrapper* );
	return p_pw->get_x_pos();
}

double Position2DBridge::get_y_pos( )
{
	DEF_WRAPPER( p_pw , IPosition2DWrapper* );
	return p_pw->get_y_pos();
}

double Position2DBridge::get_yaw( )
{
	DEF_WRAPPER( p_pw , IPosition2DWrapper* );
	return p_pw->get_yaw();
}

// ------------------- output streams  --------------------------------
std::ostream& std::operator<<( std::ostream& os , RangerBridge &rb )
{
	os<<"Ranger Bridge Data:"<<std::endl;
	for( size_t i = 0; i<rb.ranger_num() ;i++ )
		os<<"["<<i<<"]("<<R2D(rb.get_pose(i)._rz)<<")"<<rb[i]<<" ";
	os<<std::endl;
	return os;
}

std::ostream& std::operator<<( std::ostream& os, Position2DBridge &pb )
{
	os<<" Pose[ x="<<pb.get_x_pos();
	os<<" y="<<pb.get_y_pos();
	os<<" yaw="<<pb.get_yaw()<<" ]"<<std::endl;
	return os;
}
