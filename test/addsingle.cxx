/*
 * =====================================================================================
 *
 *       Filename:  dmtest.cxx
 *
 *    Description:  距离网格地图测试
 *
 *        Version:  1.0
 *        Created:  2012年12月21日 10时46分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using  namespace std;

#include <updatevector.h>
#include <distancegrid.h>
#include <himmgrid.h>
#include <himmuvgen.h>
#include <dmmbuilder.h>
#include <misc.h>
using namespace SlamLab;

int main()
{
	// 更新向量：
	uvectors_t uvecs;
	update_vector_t uv;
	// 地图
	HIMMGrid* p_cmap=NULL;
	DistanceMap* p_dmap=NULL;
	// 地图构建器
	DMMBuilder* p_dmmbd=NULL;

	// 更新时间
	struct timeval tm_start;
	struct timeval tm_end;

	// 测试次数：
	static const size_t TNUM = 100;
	// 更新坐标设置：
	uv.x = 0.0;
	uv.y = 0.0;
	// 变更活动窗口尺寸
	for( int wsz = 10 ; wsz <=16 ; wsz+=2 )
	{
		// 新地图构建器
		p_dmmbd = new DMMBuilder( 16 , 6 , 10 , double(wsz) );
		cout<<"--------- win_size:"<<wsz<<"--------"<<endl;

		// 结果输出文件：
		char fname[128];
		memset( fname , 0 , sizeof(char)*128 );
		sprintf( fname , "win%d.dat" , wsz );
		ofstream datafile( fname );

		// 变更单元格大小
		for( int csz = 0 ; csz < 9 ; csz++)
		{
			double cell_size = 0.1 + 0.05*csz;
			// 新地图：
			p_cmap = new HIMMGrid( 20.0 , 20.0, cell_size , Point2D<double>( -10.0, -10.0) );
			p_cmap->set_all_val(0);
			p_dmap = new DistanceMap( *p_cmap );
			double sum_tm = 0.0;
			// 重复TNUM次测量并求时间和
			for( size_t i =0 ; i< TNUM ; i++ )
			{
				uv.delta = 13;
				uvecs.push_back( uv );
				// 更新时间测量
				gettimeofday( &tm_start , NULL );
				uvecs>>(*p_dmmbd)(*p_cmap,0.0,0.0)>>(*p_dmap);
				gettimeofday( &tm_end , NULL );
				sum_tm += tm_end - tm_start;
				// 清除障碍
				uv.delta = -13;
				uvecs.push_back( uv );
				uvecs>>(*p_dmmbd)(*p_cmap,0.0,0.0)>>(*p_dmap);
			}
			double atime = sum_tm*1000/TNUM;
			cout<<" cell_size="<<cell_size<<"   time="<<atime<<"ms"<<endl;
			datafile<<cell_size<<"     "<<atime<<endl;
			delete p_cmap;
			delete p_dmap;
		}
		datafile.close();
		delete p_dmmbd;
	}
	return EXIT_SUCCESS;
}
