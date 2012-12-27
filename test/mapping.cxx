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
enum _CONST
{
	POS2D_INDEX = 0,
	RANGER_INDEX =0,
};

// 目标位置坐标：
static const size_t pos_num = 18;
static const Position2D positions[] = { Position2D(-7,6) ,Position2D(-6,5.5), Position2D(-2.5,5.5),
										Position2D(-2,3) ,Position2D( -2,7 ) , Position2D(0,7), 
										Position2D( 6, 6 ) , Position2D(6.5,4.5) , Position2D(0,2.5),
										Position2D(-2,2) , Position2D(-2,-2) , Position2D(0,-1.5), 
										Position2D(5,-1.5) , Position2D(7,-4), Position2D(4,-4),
										Position2D(3,-6.5), Position2D(0,-6.5),Position2D(-7,-7) };
static size_t pos_index = 0;

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
	// 初始化向量更新器及地图构建器
	HIMMUVGen himm_uvgen( 3, 1 , 5.0 );
	uvectors_t update_vectors;
	DMMBuilder dmm_builder( 16 , 6 , 10, 10.0 );
	// 地图初始化
	HIMMGrid map(20.0,20.0,0.2,Point2D<double>(-10.0,-10.0));
	DistanceMap dmap( map );
	map.set_all_val(0);
	// 初始化地图服务器：
	MapServer map_sv;
	map_sv.init();
	map_sv.start();
	// 行走控制结构
	SteerCtrl stc;
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
		//ranger_bridge>>map_builder>>map;
		ranger_bridge>>himm_uvgen( pos2d_bridge ,map )>>update_vectors>>dmm_builder( map , rx , ry )>>dmap;
		// 在控制台显示地图：
		//cout<<map;
		//cout<<dmap;
		map_sv<<map;
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
			if( pos_index >= pos_num )
				pos_index = 0;
				//break;
			
		}
		else
			pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );

	}
	//停止地图服务器
	map_sv.stop();
/* 
	HIMMGrid2Char hg2char;
	hg2char.set_map( map , 0 );
	char* p_data = hg2char.data();
	Char2HIMMGrid char2hg;
	HIMMGrid gmap;
	char2hg.set_data( p_data , gmap );
	cout<< gmap;
	*/
	// 地图数据的保存
	ofstream mapfile( "cvgrid.cmap" );
	mapfile<<map;
	mapfile.close();
	ifstream imapfile( "cvgrid.cmap" );
	HIMMGrid mp;
	imapfile>>mp;
	cout<<mp;
	imapfile.close();
	// 距离网格地图测试
	ofstream dmapfile( "dmgrid.dmap" );
	dmapfile<<dmap;
	dmapfile.close();
	ifstream idmapfile( "dmgrid.dmap" );
	DistanceMap idmap;
	idmapfile>>idmap;
	cout<<idmap;
	idmapfile.close();
	return EXIT_SUCCESS;
}

