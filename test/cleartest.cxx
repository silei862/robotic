/*
 * =====================================================================================
 *
 *       Filename:  cleartest.cxx
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
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <vector>
using  namespace std;

#include <updatevector.h>
#include <distancegrid.h>
#include <himmgrid.h>
#include <himmuvgen.h>
#include <dmmbuilder.h>
#include <misc.h>
using namespace SlamLab;

typedef SlamLab::Point2D<double> obpos;
static const size_t POS_NUM=8;
static const obpos clpos[ POS_NUM ] ={ obpos( 6 , 6 ) , obpos( -6 , -6 ), \
									   obpos( 6 , 0 ) , obpos( -6 , 0  ) , \
									   obpos( 3 , 3 ) , obpos( -3 , -3 ) , \
									   obpos( 0 , 3 ) , obpos(  3 ,  0 ) };

int main()
{
	uvectors_t uvecs;
	DMMBuilder dmm_builder( 16 , 6 , 10, 10.0 );
	HIMMGrid map(20.0,20.0,0.2,Point2D<double>(-10.0,-10.0));
	DistanceMap dmap( map );
	map.set_all_val(0);
	
	// 更新时间
	struct timeval tm_start;
	struct timeval tm_end;

	// 更新向量
	update_vector_t uv;
	uv.delta=13;
	uv.x = 0.0;
	uv.y = 0.0;
	uvecs.push_back( uv );
	uvecs>>dmm_builder( map,0.0,0.0 )>>dmap;
	// 测试次数：
	static const size_t TNUM = 100;

	// 结果输出文件
	ofstream datafile( "cleartest.dat" );

	for( size_t ob_num = 1 ; ob_num <= POS_NUM ; ob_num++ )
	{
		double sum_tm = 0.0;
		for( size_t i =0 ; i< TNUM ; i++ )
		{
			// 加入障碍,并更新
			uv.delta = 13;
			for( size_t pos_idx = 0 ; pos_idx < ob_num ; pos_idx++ )
			{
				uv.x = clpos[pos_idx]._x;
				uv.y = clpos[pos_idx]._y;
				uvecs.push_back( uv );
			}
			uvecs>>dmm_builder(map,0.0,0.0)>>dmap;
			// 除障碍，并更新
			uv.delta = -13;
			for( size_t pos_idx = 0 ; pos_idx < ob_num ; pos_idx++ )
			{
				uv.x = clpos[pos_idx]._x;
				uv.y = clpos[pos_idx]._y;
				uvecs.push_back( uv );
			}
		
			// 更新时间测试
			gettimeofday( &tm_start , NULL );
			uvecs>>dmm_builder(map,0.0,0.0)>>dmap;
			gettimeofday( &tm_end , NULL );
			sum_tm += tm_end - tm_start;
		}
		// 计算平均值、输出：
		double atime = sum_tm*1000/TNUM;
		cout<<"["<<ob_num<<"]time:"<<atime<<endl;
		datafile<<ob_num<<"      "<<atime<<endl;
	}
	datafile.close();
	return EXIT_SUCCESS;
}
