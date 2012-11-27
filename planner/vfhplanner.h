/*
 * =====================================================================================
 *
 *       Filename:  vfhplanner.h
 *
 *    Description:  使用方位直方图的局部路径规划器
 *
 *        Version:  1.0
 *        Created:  2012年11月23日 16时57分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _VFHPLANNER_H_
#define _VFHPLANNER_H_

#include <bridge.h>
#include "planner_types.h"
#include "vfh_types.h"

namespace SlamLab
{
	void  gap_finder( PolarBinHist& r_pbh, DirectionGroup& r_dg,Position2D r_pos=Position2D(0.0,0.0) ); 

	bool safety_monitor( Position2DBridge& r_pos_br, RangerBridge& r_rg_br );
}

namespace std
{
	ostream& operator<<( ostream& os , SlamLab::DirectionGroup& r_dg );
}

#endif //_VFHPLANNER_H_
