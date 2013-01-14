/*
 * =====================================================================================
 *
 *       Filename:  mapfile.h
 *
 *    Description:  提供将网格地图存储为二进制数据文件
 *
 *        Version:  1.0
 *        Created:  2012年12月07日 20时30分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Lei.Si), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _MAPFILE_H_
#define _MAPFILE_H_

#include <stdint.h>
#include <fstream>
#include <mapconvi.h>
#include <himmgrid.h>
#include <distancegrid.h>

namespace SlamLab
{

	//int himm_savemap( HIMMGrid& r_hg ,const char* file );
	enum _MAP_ID
	{
		ID_CVMAP = 1,
		ID_DSMAP = 2,
	};
}

namespace std
{
	ofstream& operator<<( ofstream& r_ofs , SlamLab::HIMMGrid& r_hg );
	ofstream& operator<<( ofstream& r_ofs , SlamLab::DistanceMap& r_dm );
	ifstream& operator>>( ifstream& r_ifs , SlamLab::HIMMGrid& r_hg );
	ifstream& operator>>( ifstream& r_ifs , SlamLab::DistanceMap& r_dm );
}

#endif //_MAPFILE_H_

