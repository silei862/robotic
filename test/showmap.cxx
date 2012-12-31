/*
 * =====================================================================================
 *
 *       Filename:  showmap.cxx
 *
 *    Description:  通过网络接口获取地图并显示
 *
 *        Version:  1.0
 *        Created:  2012年12月27日 17时17分26秒
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
#include <debug.h>
#include <himmgrid.h>
#include <distancegrid.h>
#include <mapclient.h>
using namespace std;
using namespace SlamLab;

int main()
{
	HIMMGrid cvmap;
	DistanceMap dsmap;
	MapClient map_cl;
	for( ; ; )
	{
		try
		{
			map_cl( "127.0.0.1" )>>cvmap;
			cout<<cvmap;
			sleep(1) ;
			map_cl( "127.0.0.1" )>>dsmap;
			cout<<dsmap;
			sleep(1);
		}
		catch( MapClient::setuplink_err &e )
		{
			cout<<"Setup link error!"<<endl;
		}
		catch( MapClient::data_err &e )
		{
			cout<<map_cl.server_ret()<<endl;
		}
	}
	return EXIT_SUCCESS;
}
