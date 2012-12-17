/*
 * =====================================================================================
 *
 *       Filename:  himmgrid.h
 *
 *    Description:  网格化地图类型的定义
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 21时54分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _HIMMGRID_H_
#define _HIMMGRID_H_

#include <iostream>
#include <stdint.h>
#include "grid.h"

namespace SlamLab
{
	typedef Grid< uint8_t > HIMMGrid;
	typedef GridWin< uint8_t> HIMMWin;
}

namespace std
{
	ostream& operator<<( ostream& os , SlamLab::HIMMGrid& hg );
	ostream& operator<<( ostream& os , SlamLab::HIMMWin& hw );
}

#endif //_HIMMGRID_H_
