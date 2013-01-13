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
#include <sys/time.h>
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
/*  uv.x = 0.0;
	uv.y = 0.0;
	uvecs.push_back( uv );
	uv.x = -5;
	uv.y = 0;
	uvecs.push_back( uv );
  	uv.x = 5;
	uv.y = 0;
	uvecs.push_back( uv );
	uv.x = 0;
	uv.y = 5;
	uvecs.push_back( uv );
	uv.x = 2.5;
	uv.y = 2.5;
	uvecs.push_back( uv );
	uvecs>>dmm_builder( map,0.0,0.0 )>>dmap;
	cout<<dmap<<endl;
	*/
	// 测试次数：
	static const size_t TNUM = 100;

	// 改变更新向量位置：
	uv.x = 0;
	uv.y = -9.5;

	double sum_tm = 0.0;
	for( size_t i =0 ; i< TNUM ; i++ )
	{
		uv.delta = 13;
		uvecs.push_back( uv );
		// 更新时间测试
		gettimeofday( &tm_start , NULL );
		uvecs>>dmm_builder(map,0.0,0.0)>>dmap;
		gettimeofday( &tm_end , NULL );
		sum_tm += tm_end - tm_start;
		// 清除障碍
		uv.delta = -13;
		uvecs.push_back( uv );
		uvecs>>dmm_builder(map,0.0,0.0)>>dmap;
	}
	// 输出结果：
	cout<<"Average:"<<sum_tm*1000/double(TNUM)<<endl;
	return EXIT_SUCCESS;
}
