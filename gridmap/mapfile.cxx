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

int SlamLab::himm_savemap( HIMMGrid& r_g , const char* file_name )
{
	// 初始化文件头
	HIMMFileHeader file_header;
	file_header.id = ID_HIMM_MAP;
	file_header.width = r_g.cell_cols();
	file_header.height = r_g.cell_rows();
	file_header.cell_size = r_g.cell_size();
	// 打开文件
	fstream mapfile;
	mapfile.open( file_name , fstream::out | fstream::binary );
	// 写入头部
	mapfile.write( reinterpret_cast<char*>(&file_header), sizeof(HIMMFileHeader) );
	// 写入数据
	for( uint32_t j = 0; j < file_header.height; j++ )
		for( uint32_t i = 0; i < file_header.width; i++ )
			mapfile.put( char(r_g( i , j )) );
	mapfile.close();
	return 0;
}
