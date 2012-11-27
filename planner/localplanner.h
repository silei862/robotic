/*
 * =====================================================================================
 *
 *       Filename:  localplanner.h
 *
 *    Description:  短距离规划器
 *
 *        Version:  1.0
 *        Created:  2012年11月27日 11时13分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#include <bridge.h>
#include <misc.h>

namespace SlamLab
{
	void simple_collision_avoid( Position2DBridge& r_pb, RangerBridge& r_rb );
}
