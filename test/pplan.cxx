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
#include <fstream>
using namespace std;
#include <debug.h>
#include <exception.h>
#include <misc.h>
#include <himmgrid.h>
#include <distancegrid.h>
#include <mapfile.h>
#include <dsgplanner.h>
using namespace SlamLab;


int main()
{
	try
	{
/*		// 从文件读取概率网格地图并生成gnuplot绘图数据
		HIMMGrid cmap;
		ifstream icvmap( "cvgrid.cmap" );
		icvmap>> cmap;
		ofstream cmapdata( "cmap.dat" );
		for( uint32_t j = 0 ; j< cmap.cell_rows() ; j++ )
			for( uint32_t i = 0 ; i < cmap.cell_cols(); i++ )
			{
				if( cmap( i , j )>10 )
				{
					Point2D<double> pos = cmap.cell_coord( i , j );
					cmapdata<<pos._x<<"      "<<pos._y<<endl;
				}
			}
		cmapdata.close();*/
		// 从文件读取距离网格地图
		DistanceMap dmap;
		ifstream idsmap( "dmgrid.dmap" );
		idsmap>>dmap;
		// 显示地图，检查正确性
		cout<<dmap;
		// 读入参数：
		double sx ,sy ,sth;
		sx = 6;
		sy = -3;
		sth = 1;
		//cout<<"start_x=";cin>>sx;
		//cout<<"start_y=";cin>>sy;
		//cout<<"start_th=";cin>>sth;
		double ex , ey;
		ex = 6;
		ey = 6;
		//cout<<"end x=";cin>>ex;
		//cout<<"end y=";cin>>ey;
		// 初始化路径规划器：
		DSGGuider guider( dmap, 0.3 );
		guider.set_destination( ex , ey );
		guider.set_start( sx , sy , sth );
		// 设置启发函数参数：
		guider.set_heuristic_para( 50 , 10 , 10 );
		path_t paths;
		// 打开路径输出文件：
		ofstream pathfile( "path.dat" );
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
				pathfile<<paths[i]._x<<"      "<<paths[i]._y<<endl;
			}
			cout<<"[End]"<<endl<<endl;
			// 输出角差
			for( size_t i = 0 ; i < paths.size()-1 ; i++ )
			{
				double dx = paths[i+1]._x - paths[i]._x;
				double dy = paths[i+1]._y - paths[i]._y;
				double rad = delta2rad( dx , dy );
				cout<<R2D( rad )<<"-->";
			}
			cout<<endl;

		}
		else
			cout<<" Get path fail!" <<endl;
		pathfile.close();
	}
	catch( Exception& e )
	{
		cout<<e<<endl;
	}
	return EXIT_SUCCESS;
}
