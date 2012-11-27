/*
 * =====================================================================================
 *
 *       Filename:  vfh_types.h
 *
 *    Description:  方位直方图法所使用的类型
 *
 *        Version:  1.0
 *        Created:  2012年11月23日 13时58分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _VFH_TYPES_H_
#define _VFH_TYPES_H_

#include <types.h>
#include <vector>
#include "polarhistogram.h"
#include "mapwin.h"

namespace SlamLab
{
	// 方位直方图
	typedef PolarHistogram< int32_t > PolarHist;
	// 二值方位直方图
	typedef PolarHistogram< uint8_t > PolarBinHist;
	// 地图窗口
	typedef TMapWin< uint8_t > MapWin;
}


#endif //_VFH_TYPES_H_
