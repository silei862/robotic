/*
 * =====================================================================================
 *
 *       Filename:  mapfile.cxx
 *
 *    Description:  地图文件保存的实现
 *
 *        Version:  1.0
 *        Created:  2012年12月07日 20时47分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Lei.Si), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <fstream>
#include <stdlib.h>
#include "himmgrid.h"
#include "mapfile.h"

using namespace std;
using namespace SlamLab;

// 保存地图：
ofstream& std::operator<<( ofstream& r_ofs , HIMMGrid& r_hg )
{
	HIMMGrid2Char hg2char;
	hg2char.set_map( r_hg , 0 );
	r_ofs.write( hg2char.data() , hg2char.size() );
	return r_ofs;
}

ofstream& std::operator<<( ofstream& r_ofs , DistanceMap& r_dm )
{
	DistanceGrid2Char dm2char;
	dm2char.set_map( r_dm , 0 );
	r_ofs.write( dm2char.data() , dm2char.size() );
	return r_ofs;
}

ifstream& std::operator>>( ifstream& r_ifs , HIMMGrid& r_hg )
{
	// 获取文件大小：
	r_ifs.seekg( 0 , ios::end );
	size_t len = r_ifs.tellg();
	r_ifs.seekg( 0 , ios::beg );
	// 读入数据：
	char* p_data = new char[ len ];
	r_ifs.read( p_data , len );
	// 数据转换：
	Char2HIMMGrid char2hg;
	char2hg.set_data( p_data , r_hg );
	// 清理
	delete[] p_data;
	return r_ifs;
}

ifstream& std::operator>>( ifstream& r_ifs , DistanceMap& r_dm )
{
	// 获取文件大小：
	r_ifs.seekg( 0 , ios::end );
	size_t len = r_ifs.tellg();
	r_ifs.seekg( 0 , ios::beg );
	// 读入数据：
	char* p_data = new char[ len ];
	r_ifs.read( p_data , len );
	// 数据转换：
	Char2DistanceGrid char2dm;
	char2dm.set_data( p_data , r_dm );
	// 清理
	delete[] p_data;
	return r_ifs;
}
