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
#include <sys/time.h>
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
		// 从文件读取概率网格地图并生成gnuplot绘图数据
		HIMMGrid cmap;
		ifstream icvmap( "cvgrid.cmap" );
		if( icvmap.fail() )
			cout<<"cvgrid.cmap打开失败，将不会产生路径的背景数据!"<<endl;
		else
		{
			// 读入概率地图，并生成路径背景数据
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
			cmapdata.close();
		}

		// 从文件读取距离网格地图
		DistanceMap dmap;
		ifstream idsmap( "dmgrid.dmap" );
		if( idsmap.fail() )
		{
			cout<<"dmgrid.dmap打开失败，退出！"<<endl;
			return EXIT_FAILURE;
		}
		// 读入距离地图数据
		idsmap>>dmap;
		// 显示地图，检查正确性
		//cout<<dmap;
		// 读入参数：
		double sx ,sy ,sth;
		sx = 6;
		sy = -2.5;
		sth = 1.57;
		//cout<<"start_x=";cin>>sx;
		//cout<<"start_y=";cin>>sy;
		//cout<<"start_th=";cin>>sth;
		double ex , ey;
		ex = 0;
		ey = 7;
		//cout<<"end x=";cin>>ex;
		//cout<<"end y=";cin>>ey;
		
		// 初始化路径规划器：
		DSGGuider guider( dmap, 0.3 );
		guider.set_destination( ex , ey );
		guider.set_start( sx , sy , sth );
		
		path_t paths;
		
		// 读入参数：
		double w1, w2,w3;
		cout<<"w1=";cin>>w1;
		cout<<"w2=";cin>>w2;
		cout<<"w3=";cin>>w3;
		// 生成路径文件名：
		char pdf[128]={0};
		sprintf( pdf , "path_%d%d%d.dat", int(w1*10), int(w2*10),int(w3*10) );
		// 打开路径输出文件：
		ofstream pathfile( pdf );
		if( pathfile.fail() )
		{
			cout<<"无法创建"<<pdf<<",退出！"<<endl;
			return EXIT_FAILURE;
		}
	/*  ofstream tdfile( "path_test_time.dat" );
		// 获取路径并测量规划路径耗时：
		struct timeval start_time;
		struct timeval end_time;
		size_t NTEST = 100;
		for( int k =1 ; k < 9 ; k++ )
		{
			for( int l = 1 ; l < 10 - k ; l ++ )
			{
				double time_sum = 0;
				double kd = k*0.1;
				double ld = l*0.1;
				double dw = 1 - kd -ld ;
				bool success = false;
				for( size_t i = 0 ; i < NTEST ; i++ )
				{
					// 设置启发函数参数：
					guider.set_heuristic_para( kd , ld , dw );
					guider.set_start( -6 , -6 , -2 );
					guider.set_destination( 6 , 6 );
					gettimeofday( &start_time , NULL );
					success= guider.get_path( paths );
					gettimeofday( &end_time , NULL );
					time_sum += end_time - start_time;
					if( !success )
					{
						cout<<"Error Time!";
						return EXIT_FAILURE;
					}

				}
				double av_tm = time_sum*1000/NTEST;
				cout<<"["<<kd<<","<<ld<<","<<dw<<"] ";
				cout<<" Average Time:"<<av_tm<<endl;
				tdfile<<"w0="<<kd<<" w1="<<dw<<" w2="<<ld<<" time="<<av_tm<<"ms"<<endl;
			}
			tdfile<<endl;
		}
		tdfile.close();
*/
			guider.set_heuristic_para(w1,w3,w2);
			bool success= guider.get_path( paths );
			if( success )
			{
				cout<<"成功获取路径！"<<endl;
				cout<<"[Start]-->";
				for( size_t i =0; i<paths.size(); i++ )
				{
					cout<<"["<<paths[i]._x<<","<<paths[i]._y<<"]-->";
					pathfile<<paths[i]._x<<"      "<<paths[i]._y<<endl;
				}
				cout<<"[End]"<<endl;
			}
			else
				cout<<" Get path fail!" <<endl; 
		pathfile.close();
	}
	catch( Exception& e )
	{
		cerr<<e<<endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
