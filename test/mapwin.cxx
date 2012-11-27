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
#include <localplanner.h>

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
	PlayerClient robot( ROBOT_ADDR );	
	Position2dProxy pl_pos2d( &robot,POS2D_INDEX );
	RangerProxy pl_ranger( &robot, RANGER_INDEX );
	RangerPlayer ranger( pl_ranger);
	Position2DPlayer pos2d( pl_pos2d );
	
	RangerBridge ranger_bridge( &ranger );
	Position2DBridge pos2d_bridge( &pos2d );

	HIMMBuilder map_builder;
	map_builder.attach_position2d( pos2d_bridge );
	HIMMGrid map(20.0,20.0,0.2,Point2D<double>(-10.0,-10.0));
	map.set_all_val(0);
	SteerCtrl stc;	
	for(;;)
	{
		robot.Read();
		simple_collision_avoid( ranger_bridge , stc );
		pos2d_bridge.set_speed( stc._ahead_veloc, stc._angular_veloc );
		ranger_bridge>>map_builder>>map;
		// 获取地图窗口并输出
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		HIMMWin map_win=map.get_win(rx,ry,6.0);
		// 输出地图窗口：
		cout<<map_win<<endl;
	}
	return EXIT_SUCCESS;
}

