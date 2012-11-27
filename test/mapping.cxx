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
	// Player/Stage模拟器初始化
	PlayerClient robot( ROBOT_ADDR );	
	Position2dProxy pl_pos2d( &robot,POS2D_INDEX );
	RangerProxy pl_ranger( &robot, RANGER_INDEX );
	RangerPlayer ranger( pl_ranger);
	Position2DPlayer pos2d( pl_pos2d );
	// 初始化传感器桥接器
	RangerBridge ranger_bridge( &ranger );
	Position2DBridge pos2d_bridge( &pos2d );
	// 初始化地图构建器
	HIMMBuilder map_builder;
	map_builder.attach_position2d( pos2d_bridge );
	// 地图初始化
	HIMMGrid map(20.0,20.0,0.2,Point2D<double>(-10.0,-10.0));
	map.set_all_val(0);
	// 行走控制结构
	SteerCtrl stc;
	// 地图绘制
	for(;;)
	{
		robot.Read();
		// 绘制地图：
		ranger_bridge>>map_builder>>map;
		// 在控制台显示地图：
		cout<<map;

		// 调用防撞漫游控制计算函数：
		if(simple_collision_avoid( ranger_bridge , stc ))
		{
			pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );
			continue;
		}
		// 进行短期目标测试
		bool acheived = goto_pos(pos2d_bridge,Position2D(6,6),stc);
		if( acheived )
			break;
		else
			pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );

	}
	return EXIT_SUCCESS;
}

