/*
 * =====================================================================================
 *
 *       Filename:  pplan.cxx
 *
 *    Description:  路径搜寻测试程序
 *
 *        Version:  1.0
 *        Created:  2012年12月31日 22时40分10秒
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
using namespace std;
#include <debug.h>
#include <exception.h>
#include <distancegrid.h>
#include <mapfile.h>
//#include <hybirdastar.h>
#include <dsgplanner.h>
using namespace SlamLab;


int main()
{
	try
	{

		// 从文件读取距离网格地图
		DistanceMap dmap;
		ifstream idsmap( "dmgrid.dmap" );
		idsmap>>dmap;
		// 显示地图，检查正确性
		cout<<dmap;
		// 读入参数：
		double sx ,sy ,sth;
		cout<<"start_x=";cin>>sx;
		cout<<"start_y=";cin>>sy;
		cout<<"start_th=";cin>>sth;
		double ex , ey;
		cout<<"end x=";cin>>ex;
		cout<<"end y=";cin>>ey;
		// 初始化路径规划器：
		DSGGuider guider( dmap, 0.3 );
		guider.set_destination( ex , ey );
		guider.set_start( sx , sy , sth );
		path_t paths;
		// 获取路径：
		DBG_INFO( "========== Before Get Control ==========" );
		bool success= guider.get_path( paths );
		if( success )
		{
			cout<<" Success get path! "<<endl;
			cout<<"[Start]-->";
			for( size_t i =0; i<paths.size(); i++ )
			{
			
				cout<<"["<<paths[i]._x<<","<<paths[i]._y<<"]-->";
			}
			cout<<"[End]"<<endl;
		}
		else
			cout<<" Get path fail!" <<endl;
	}
	catch( Exception& e )
	{
		cout<<e<<endl;
	}
	return EXIT_SUCCESS;
}
