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

namespace SlamLab
{
	enum _MAP_ID
	{
 		ID_HIMM_MAP = 0x55AA0001,
	};



	typedef struct _HIMMFileHeader 
	{
		uint32_t id;
		uint32_t width;
		uint32_t height;
		double cell_size;
	} HIMMFileHeader;

	int himm_savemap( HIMMGrid& r_hg ,const char* file );
}

#endif //_MAPFILE_H_

