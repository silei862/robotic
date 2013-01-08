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
#include <string>
#include <pthread.h>
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
#include <dsgplanner.cxx>
#include <mapfile.h>
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

// 用户输入控制：
struct usercontrol_t
{
	double _x;
	double _y;
	bool _valnew;
	bool _exit;
};

// 用户输入控制线程：
void* user_control( void* p_data )
{
	usercontrol_t* p_ctrl = reinterpret_cast< usercontrol_t* >( p_data );
	string ex_query; 
	while( true )
	{
		// 接受用户输入新坐标：
		cout<<" x=";cin>>p_ctrl->_x;
		cout<<" y=";cin>>p_ctrl->_y;
		p_ctrl->_valnew = true;
		// 退出询问：
		cout<<" Exit Planning?";cin>>ex_query;
		if( ex_query == "y" )
		{
			p_ctrl->_exit = true;
			break;
		}
	}
	return (void*)( NULL );
}


int main( int argc, char* argv[] )
{
	try
	{
		// ----------------------- 初始化部分 --------------------------------------------
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
		// 输入地图：
		ifstream incmapfile( "cvgrid.cmap" );
		ifstream indmapfile( "dmgrid.dmap" );
		incmapfile>>map;
		indmapfile>>dmap;

		// 行走控制结构
		SteerCtrl stc;
		stc._ahead_veloc = 0;
		stc._angular_veloc = 0;
	
		// 创建用户输入
		usercontrol_t user_ctrl;
		user_ctrl._exit = false;
		user_ctrl._valnew = false;
		// 用户输入线程：
		pthread_t userctrl;
		int rc = pthread_create( &userctrl , NULL , user_control , (void*)(&user_ctrl) );
		if( rc < 0 )
			cout<<" Create user control thread fail! "<<endl;
		
		// 路径：
		path_t gpath;
		gpath.clear();
		// 导航控制变量
		bool guiding = false;
		size_t pos_index = 0;
		// 导航器初始化：
		DSGGuider guider( dmap , 0.3 );

		// 导航计算：
		while( ! user_ctrl._exit )
		{
			robot.Read();
			double rx=pos2d_bridge.get_x_pos();
			double ry=pos2d_bridge.get_y_pos();
			double rth=pos2d_bridge.get_yaw();
			// 绘制地图：
			//ranger_bridge>>map_builder>>map;
			ranger_bridge>>himm_uvgen( pos2d_bridge ,map )>>update_vectors>>dmm_builder( map , rx , ry )>>dmap;
			// 检查数据用户输入
			if( user_ctrl._valnew && !guiding )
			{
				// 输入合法性检查：
				if( dmap.in( user_ctrl._x , user_ctrl._y ) && dmap( user_ctrl._x , user_ctrl._y )._d > 0.45 )
				{
					guider.set_destination( user_ctrl._x , user_ctrl._y );
					guider.set_start( rx , ry , rth ); 
					// 获取路径,如果成功则启动导航：
					if( guider.get_path( gpath ) )
					{
						pos_index = 0;
						guiding = true;
						// 输出路径：
						cout<<"Start-->";
						for( size_t i = 0 ; i < gpath.size() ; i++ )
							cout<<"["<<gpath[i]._x<<","<<gpath[i]._y<<"]-->";
						cout<<"End"<<endl;
					}
					else
						cout<<"Destination unreachable!"<<endl;
				}
				else
					cout<<" Destination Error !"<<endl;
				// 清除新数据标志：
				user_ctrl._valnew = false;
			}

			// 调用防撞漫游控制计算函数：
			if(simple_collision_avoid( ranger_bridge , stc ))
			{
				//cout<<" inovke collision avoid!"<<endl;
				pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );
				continue;
			}
			if( guiding )
			{
				// 按照路径进行导航：
				bool acheived = goto_pos( pos2d_bridge,gpath[pos_index],stc, 0.3 );
				if( acheived )
				{
					cout<<"["<<gpath[pos_index]._x<<","<<gpath[pos_index]._y<<"] reached!"<<endl;
					pos_index ++;
					if( pos_index > gpath.size() )
					{
						pos_index = 0;
						guiding = false;
					}
				}
				else
					pos2d_bridge.set_speed( stc._ahead_veloc , stc._angular_veloc );
			}
		}
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
	}
	catch( Exception& e )
	{
		cout<<e<<endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

