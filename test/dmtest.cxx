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
#include <iostream>
#include <vector>
using  namespace std;

#include <updatevector.h>
#include <distancegrid.h>
#include <himmgrid.h>
#include <himmuvgen.h>
#include <dmmbuilder.h>
using namespace SlamLab;

int main()
{
	uvectors_t uvecs;
	DMMBuilder dmm_builder( 16 , 6 , 10, 10.0 );
	HIMMGrid map(20.0,20.0,0.5,Point2D<double>(-10.0,-10.0));
	DistanceMap dmap( map );
	map.set_all_val(0);

	update_vector_t uv;
	uv.x = -3.0;
	uv.y = 0.0;
	uv.delta = 13;
	uvecs.push_back( uv );
	uv.x = 1.0;
	uvecs.push_back( uv );
	uvecs>>dmm_builder(map,0.0,0.0)>>dmap;
	cout<<dmap;
	uv.delta = -10;
	uvecs.push_back( uv );
	cout<<" after push uv!"<<endl;
	uvecs>>dmm_builder( map , 0.0,0.0)>>dmap;
	cout<<dmap;
	return EXIT_SUCCESS;
}
