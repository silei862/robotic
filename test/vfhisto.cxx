/*
* =====================================================================================
*
*       Filename:  main.cc
*
*    Description:  Mapping by laser
*
*        Version:  1.0
*        Created:  2012年04月06日 13时39分10秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  思磊 (Silei), silei862@gmail.com
*   Organization:  TSA.PLA.CN
*
* =====================================================================================
*/
#include <stdlib.h>
#include <iostream>
#include <libplayerc++/playerc++.h>
#include <playerwrapper.h>
#include <himmbuilder.h>
#include <bridge.h>
#include <himmgrid.h>
#include <misc.h>
#include <vfh.h>

using namespace SlamLab;
using namespace std;
using namespace PlayerCc;

#define ROBOT_ADDR "localhost"
enum _CONST
{
	POS2D_INDEX = 0,
	RANGER_INDEX =0,
};

int main( int argc, char* argv[] )
{
	// 初始化Player/Stage模拟器
	PlayerClient robot( ROBOT_ADDR );	
	Position2dProxy pl_pos2d( &robot,POS2D_INDEX );
	RangerProxy pl_ranger( &robot, RANGER_INDEX );
	RangerPlayer ranger( pl_ranger);
	Position2DPlayer pos2d( pl_pos2d );
	// 初始化桥接器
	RangerBridge ranger_bridge( &ranger );
	Position2DBridge pos2d_bridge( &pos2d );
	// 初始化地图构建器
	HIMMBuilder map_builder;
	map_builder.attach_position2d( pos2d_bridge );
	// 初始化地图
	HIMMGrid map(20.0,20.0,0.1,Point2D<double>(-10.0,-10.0));
	map.set_all_val(0);
	
	// 初始化方位直方图构建器和方位直方图
	PHBuilder ph_builder(0.5,0.5);
	PolarHist ph(72);

	for(;;)
	{
		robot.Read();
		ph.set_all_val(0);
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		double yaw=pos2d_bridge.get_yaw();
	//	simple_collision_avoid( pos2d_bridge, ranger_bridge );
		pos2d_bridge.set_speed( 0.0,0.7 );
		ranger_bridge>>map_builder>>map>>ph_builder( rx,ry,4.0)>>ph;
		// 输出直方图
		cout<<ph<<endl; 
	}
	return EXIT_SUCCESS;
}

