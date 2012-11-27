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
#include <vfhplanner.h>

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
	HIMMGrid map(20.0,20.0,0.2,Point2D<double>(-10.0,-10.0));
	map.set_all_val(0);
	
	// 初始化方位直方图构建器和方位直方图
	PHBuilder ph_builder(0.5,0.5);
	PolarHist ph(72);
	// 初始化二值方位直方图构建器和结果容器
	BinPHBuilder bin_ph_builder;
	PolarBinHist bin_ph(72);
	bin_ph.set_all_val(1);
	// 对环境预先扫描
	for( int i=0;i<100;i++ )
	{
		robot.Read();
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		double yaw=pos2d_bridge.get_yaw();
		pos2d_bridge.set_speed( 0.0,0.7 );
		ranger_bridge>>map_builder>>map>>ph_builder( rx,ry,6.0)>>ph>>bin_ph_builder(10)>>bin_ph;
	}
	// 正式开始运行
	DirectionGroup dds;
	// 读入目标坐标：
	Position2D dist_pos(6,6);
	cout<<" x=";cin>>dist_pos._x;
	cout<<" y=";cin>>dist_pos._y;

	for(;;)
	{
		robot.Read();
		double rx=pos2d_bridge.get_x_pos();
		double ry=pos2d_bridge.get_y_pos();
		double yaw=pos2d_bridge.get_yaw();
		// 计算目标方向值：
		double delta_x = dist_pos._x - rx;
		double delta_y = dist_pos._y - ry;
		double distance = sqrt( delta_x*delta_x + delta_y*delta_y);
		if(distance < 0.5 )
		{
			cout<<" x=";cin>>dist_pos._x;
			cout<<" y=";cin>>dist_pos._y;
		 	delta_x = dist_pos._x - rx;
		 	delta_y = dist_pos._y - ry;
		 	distance = sqrt( delta_x*delta_x + delta_y*delta_y);
		}
		//simple_collision_avoid( pos2d_bridge, ranger_bridge );
		//pos2d_bridge.set_speed( 0.0,0.6 );
		ranger_bridge>>map_builder>>map>>ph_builder( rx,ry,6.0)>>ph>>bin_ph_builder(10)>>bin_ph;
		dds.clear();
		gap_finder( bin_ph , dds , Position2D( rx ,ry ) );
		double beta = acos( delta_x/distance );
		if( delta_y< 0 )
			beta = 2*PI - beta;
		double cyaw=yaw;
		if( cyaw <0 )
			cyaw+=2*PI;
		//挑选佳方向：
		double min_diff=2*PI;
		size_t min_index=0;
		for( size_t i =0;i<dds.size();i++ )
			if( dds[i]._size < 25 )
				continue;
			else
			{
				double dir_diff = fabs( dds[i]._direction - beta );
				if( min_diff > dir_diff )
				{
					min_diff = dir_diff;
					min_index = i;
				}
			}
		if( min_diff == 0 )
		{
			pos2d_bridge.set_speed( 0.0,0.7);
			continue;
		}

		double ahead_dir=dds[min_index]._direction;
		double dir_diff=ahead_dir - cyaw;
		if( fabs(dir_diff) > PI )
			dir_diff=( dir_diff < 0 )?2*PI+dir_diff:dir_diff-2*PI;
		double turn_rate=dir_diff/2;
		pos2d_bridge.set_speed( 0.3, turn_rate );
		INFO_VAR( R2D(ahead_dir) );
		INFO_VAR( R2D( cyaw ) );
		INFO_VAR( R2D( beta ) );
		cout<<dds<<endl; 
	}
	return EXIT_SUCCESS;
}

