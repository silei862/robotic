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
	// 初始化路径规划器：
	DSGGuider dsguider( dmap );
	ctrlgroup_t ctrl_group;
	dsguider.set_start( -6,-6,0.4 );
	dsguider.set_destination( 6,6,0.3);
	// 获取路径：
	DBG_INFO( "========== Before Get Control ==========" );
	bool success=dsguider.get_controls( ctrl_group );
	if( success )
		cout<<" Success get path! "<<endl;
	else
		cout<<" Get path fail!" <<endl;

	return EXIT_SUCCESS;
}
