/*
* =====================================================================================
*
*       Filename:  mappingtime.cxx
*
*    Description:  地图生成时间测试
*
*        Version:  1.0
*        Created:  2013年01月06日 10时19分03秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  思磊 (Silei), silei862@gmail.com
*   Organization:  TSA.PLA.CN
*
* =====================================================================================
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
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
// 环境选择
#define EVIRONMENT 1
// 噪声开关
 #define NO_RANGER_NOISE
 #define NO_POS_NOISE

enum _CONST
{
	POS2D_INDEX = 0,
	RANGER_INDEX =0,
};

// 目标位置坐标：
#if EVIRONMENT == 1
static const size_t pos_num = 18;
static const Position2D positions[] = { Position2D(-7,6) ,Position2D(-6,5.5), Position2D(-2.5,5.5),
										Position2D(-2,3) ,Position2D( -2,7 ) , Position2D(0,7), 
										Position2D( 6, 6 ) , Position2D(6.5,4.5) , Position2D(0,2.5),
										Position2D(-2,2) , Position2D(-2,-2) , Position2D(0,-1.5), 
										Position2D(5,-1.5) , Position2D(7,-4), Position2D(4,-4),
										Position2D(3,-6.5), Position2D(0,-6.5),Position2D(-7,-7) };
#endif // EVIRONMENT == 1

#if EVIRONMENT == 2
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
#endif // EVIRONMENT == 2

static size_t pos_index = 0;

int main( int argc, char* argv[] )
{
	try
	{
		// 参数
		string filename;
		if( argc>1 )
			filename = argv[1];
		else
			filename = "evupdatetime";

		// Player/Stage模拟器初始化
		PlayerClient robot( ROBOT_ADDR );	
		Position2dProxy pl_pos2d( &robot,POS2D_INDEX );
		RangerProxy pl_ranger( &robot, RANGER_INDEX );
		// 传感器初始化：
#ifdef NO_RANGER_NOISE
		RangerPlayer ranger( pl_ranger);
#else
		RangerNoisePlayer ranger( pl_ranger, 0.05 );
#endif // NO_RANGER_NOISE

#ifdef NO_POS_NOISE
		Position2DPlayer pos2d( pl_pos2d );
#else
		Position2DNoisePlayer pos2d( pl_pos2d , 0.01 , 0.05, 0.01 , 0.01 );
#endif // NO_POS_NOISE

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
		// 圈数计数器
		size_t cir_counter = 1;
		static const size_t MAX_CIR =10;
		// 时间计算参数
		double time_sum = 0;
		size_t time_count = 0;
		struct timeval start_time;
		struct timeval end_time;
		// 数据输出文件：
		filename +=".dat";
		ofstream datafile(filename.c_str());	
		if( datafile.fail() )
		{
			cout<<"创建数据文件失败！"<<endl;
			return EXIT_FAILURE;
		}
	
		// 地图绘制
		for(;;)
		{
			robot.Read();
			double rx=pos2d_bridge.get_x_pos();
			double ry=pos2d_bridge.get_y_pos();
			// 绘制地图：
			ranger_bridge>>himm_uvgen( pos2d_bridge ,map )>>update_vectors;
			// 计算地图更新所用时间：
			gettimeofday( &start_time , NULL );
			update_vectors>>dmm_builder( map , rx , ry )>>dmap;
			gettimeofday( &end_time , NULL );
			double time_delta = end_time - start_time;
			time_sum += end_time - start_time;
			time_count++;
			//cout<<"["<<cir_counter<<"]utime="<<time_delta*1000000<<endl;

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
					//计算更新时间均值：
					double atime = time_sum*1000000/time_count;
					cout<<"["<<cir_counter<<"] average time="<<atime<<"us"<<endl;
					datafile<<cir_counter<<"   "<<atime<<endl;
					// 更新圈数：
					cir_counter++;
					if( cir_counter > MAX_CIR )
						break;

					//复位所有状态量：
					time_sum = 0.0;
					time_count = 0;
				}
			}
			else
				pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );

		}
		datafile.close();
	}
	catch( Exception& e )
	{
		cerr<<e<<endl;
		return EXIT_FAILURE;
	}
	catch(PlayerCc::PlayerError& e )
	{
		cerr<<e<<endl;
		return EXIT_FAILURE;	
	}
	return EXIT_SUCCESS;
}

