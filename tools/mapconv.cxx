/*
 * =====================================================================================
 *
 *       Filename:  convert.cxx
 *
 *    Description:  根据地图文件生成GNUPlot数据
 *
 *        Version:  1.0
 *        Created:  2013年01月14日 10时37分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include <himmgrid.h>
#include <distancegrid.h>
#include <mapconvi.h>
#include <mapfile.h>
using namespace SlamLab;

void print_usage()
{
	cout<<"用法："<<endl;
	cout<<"   mapconv <mapfile> "<<endl<<endl;
}

int main( int argc , char* argv[] )
{
	// 参数检查：
	if( argc<2 )
	{
		cout<<"请指定地图文件！"<<endl<<endl;
		print_usage();
		cout<<endl;
		return EXIT_SUCCESS;
	}

	// 载入地图文件
	ifstream mapfile;
	mapfile.open( argv[1] );
	if( mapfile.fail() )
	{
		cout<<"文件"<<argv[1]<<"不存在！"<<endl;
		return EXIT_FAILURE;
	}
	
	//计算文件大小以准备缓冲区：
	mapfile.seekg( 0 , ios::end );
	size_t file_len = mapfile.tellg();
	mapfile.seekg( 0 , ios::beg );
	
	// 基本文件名：
	string base_name( argv[1] );

	// 申请缓冲区
	char* p_data = new char[ sizeof(char)*file_len ];
	if( NULL == p_data )
	{
		cout<<"内存申请失败！"<<endl;
		return EXIT_FAILURE;
	}

	// 读取文件至缓冲：
	mapfile.read( p_data, file_len );
	grid_head_t* p_head = reinterpret_cast< grid_head_t* >( p_data );

	// 生成文件名：
	string file_name = base_name + ".dat";
	ofstream datafile( file_name.c_str() );
	if( datafile.fail() )
	{
		cout<<"创建数据失败！"<<endl;
		mapfile.close();
		delete p_data;
		return EXIT_FAILURE;
	}

	// 概率网格地图：
	HIMMGrid himmgrid;
	// 距离网格地图：
	DistanceMap dismap;

	// 根据文件类型加以处理：
	switch( p_head->_id )
	{
		case ID_CVMAP:
			mapfile>>himmgrid;
			// 写入数据：
			for( int j = 0 ; j < himmgrid.cell_rows() ; j++ )
			{
				for( int i = 0 ; i < himmgrid.cell_cols() ; i++ )
					datafile<<int(himmgrid( uint32_t(i), uint32_t(j) ))<<" ";
				datafile<<endl;
			}
			break;

		case ID_DSMAP:
			mapfile>>dismap;
			// 写入数据：
			for( int j = 0; j < dismap.rows() ; j++ )
			{
				for( int i = 0 ; i < dismap.cols(); i ++ )
					datafile<<(dismap( size_t( i ) , size_t( j ) )._d)<<" ";
				datafile<<endl;
			}
			break;

		default:
			cout<<"未知地图类型！"<<endl;
			return EXIT_SUCCESS;
	}

	// 清理数据：
	delete p_data;
	mapfile.close();
	datafile.close();
}
