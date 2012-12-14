/*
 * =====================================================================================
 *
 *       Filename:  distancegrid.h
 *
 *    Description:  距离网格地图
 *
 *        Version:  1.0
 *        Created:  2012年12月14日 09时40分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _DISTANCEGRID_H_
#define _DISTANCEGRID_H_

#include <coord.h>

typedef struct _dm_cell_t
{
	double 				val;
	Position2D<double>	ob_pos;
} dm_cell_t

typedef Grid<dm_cell_t> DMGrid;

#endif //_DISTANCEGRID_H_
