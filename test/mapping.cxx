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
#include <string>
#include <iostream>
#include <libplayerc++/playerc++.h>
#include <playerwrapper.h>
#include <himmbuilder.h>
#include <dmmbuilder.h>
#include <himmuvgen.h>
#include <distancegrid.h>
#include <bridge.h>
#include <himmgrid.h>
#include <misc.h>
#include <vfh.h>
#include <localplanner.h>
#include <mapfile.h>
#include <mapserver.h>
#include <mapconvi.h>

using namespace SlamLab;
using namespace std;
using namespace PlayerCc;

#define ROBOT_ADDR "localhost"
#define DEF_FILE_NAME "map"
enum _CONST
{
	POS2D_INDEX = 0,
	RANGER_INDEX =0,
};

// 目标位置坐标：
/*static const size_t pos_num = 18;
static const Position2D positions[] = { Position2D(-7,6) ,Position2D(-6,5.5), Position2D(-2.5,5.5),
										Position2D(-2,3) ,Position2D( -2,7 ) , Position2D(0,7), 
										Position2D( 6, 6 ) , Position2D(6.5,4.5) , Position2D(0,2.5),
										Position2D(-2,2) , Position2D(-2,-2) , Position2D(0,-1.5), 
										Position2D(5,-1.5) , Position2D(7,-4), Position2D(4,-4),
										Position2D(3,-6.5), Position2D(0,-6.5),Position2D(-7,-7) };
*/
static const size_t pos_num = 28;
static const Position2D positions[] = { Position2D(1,-4.5) ,Position2D(8,-3.8),Position2D(-8.5,-3.8),
										Position2D(-8.5,5.7),Position2D(-5.7,6),Position2D(-6,9),
										Position2D(-9,9),Position2D(-5.7,6.5),Position2D(-6,5.7),
										Position2D(-4.2,6),Position2D(-4.2,9),Position2D(-1.5,8),
										Position2D(-4.2,7),Position2D(-4.2,6),Position2D(0,5),
										Position2D(4,6),Position2D(2.5,8.5),Position2D(0.5,5),
										Position2D(-3.5,4.5),Position2D(-4,-2),Position2D(0.8,0),
										Position2D(1.5,3),Position2D(3.5,2),Position2D(0.8,1),
										Position2D(0.8,-1),Position2D(4,-1),Position2D(1,-4),
										Position2D(1,-9) };

static size_t pos_index = 0;

int main( int argc, char* argv[] )
{
	// 设置地图输出文件名：
	string filename;
	if( argc >1 )
		filename = argv[1];
	else
		filename = DEF_FILE_NAME;

	// Player/Stage模拟器初始化
	PlayerClient robot( ROBOT_ADDR );	
	Position2dProxy pl_pos2d( &robot,POS2D_INDEX );
	RangerProxy pl_ranger( &robot, RANGER_INDEX );

	// 传感器：
	RangerPlayer ranger( pl_ranger);
	//RangerNoisePlayer ranger( pl_ranger, 0.10 );
	Position2DPlayer pos2d( pl_pos2d );
	//Position2DNoisePlayer pos2d( pl_pos2d , 0.01 , 0.05, 0.01 , 0.01 );
	
	// 初始化传感器桥接器
	RangerBridge ranger_bridge( &ranger );
	Position2DBridge pos2d_bridge( &pos2d );
	
	// 初始化向量更新器及地图构建器
	HIMMUVGen himm_uvgen( 3, 1 , 5.0 );
	uvectors_t update_vectors;
	DMMBuilder dmm_builder( 16 , 6 , 10, 10.0 );
	
	// 地图初始化
	HIMMGrid cmap(22.0,22.0,0.2,Point2D<double>(-11.0,-11.0));
	DistanceMap dmap( cmap );
	cmap.set_all_val(0);
	
	// 初始化地图服务器：
	MapServer map_sv;
	map_sv.init();
	map_sv.start();
	
	// 行走控制结构
	SteerCtrl stc;
	
	// 圈数计数器：
	size_t cir_count = 0;
	static const size_t MAX_CIR = 1; 

	// 对环境预先扫描
	for( int i=0;i<10;i++ )
	{
		robot.Read();
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		double yaw=pos2d_bridge.get_yaw();
		pos2d_bridge.set_speed( 0.0,0.7 );
	}
	
	// 地图绘制
	for(;;)
	{
		robot.Read();
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		
		// 绘制地图：
		//ranger_bridge>>map_builder>>cmap;
		ranger_bridge>>himm_uvgen( pos2d_bridge ,cmap )>>update_vectors>>dmm_builder( cmap , rx , ry )>>dmap;
		
		// 在控制台显示地图：
		//cout<<cmap;
		//cout<<dmap;
		map_sv<<cmap;
		map_sv<<dmap;
		
		// 调用防撞漫游控制计算函数：
		if(simple_collision_avoid( ranger_bridge , stc ))
		{
			pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );
			continue;
		}
		
		// 进行短期目标测试
		bool acheived = goto_pos(pos2d_bridge,positions[pos_index],stc);
		if( acheived )
		{
			pos_index ++;
			// 已经跑完一圈
			if( pos_index >= pos_num )
			{
				pos_index = 0;
				cir_count++;
				if( cir_count >= MAX_CIR )
					break;
			}
		}
		else
			pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );

	}
	
	// 文件名的生成：
	string cvfile = filename + ".cmap";
	string dsfile = filename + ".dmap";
	// 保存概率地图
	ofstream cmapfile( cvfile.c_str() );
	cmapfile<<cmap;
	cmapfile.close();
	// 保存距离地图
	ofstream dmapfile( dsfile.c_str() );
	dmapfile<<dmap;
	dmapfile.close();
	//停止地图服务器
	map_sv.stop();
	return EXIT_SUCCESS;
}

