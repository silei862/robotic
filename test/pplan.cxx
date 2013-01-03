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
#include <distancegrid.h>
#include <mapfile.h>
#include <dsgplanner.h>
using namespace SlamLab;


int main()
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
	double x = sx;
	double y = sy;
	double th = sth;
	double t;
	// 初始化路径规划器：
	DSGGuider dsguider( dmap );
	ctrlgroup_t ctrl_group;
	dsguider.set_start(sx , sy, sth );
	dsguider.set_destination( 6,6,0.3);
	// 获取路径：
	DBG_INFO( "========== Before Get Control ==========" );
	bool success=dsguider.get_controls( ctrl_group );
	if( success )
	{
		cout<<" Success get path! "<<endl;
		cout<<"[Start]-->";
		for( size_t i =0; i<ctrl_group.size(); i++ )
		{
			
			cout<<"["<<ctrl_group[i]._v<<","<<ctrl_group[i]._w<<","<<ctrl_group[i]._tl<<"]-->";
		}
		cout<<"[End]"<<endl;
	}
	else
		cout<<" Get path fail!" <<endl;

	return EXIT_SUCCESS;
}
