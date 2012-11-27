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
#include <types.h>
#include <misc.h>

namespace SlamLab
{	
	typedef struct
	{
		double _ahead_veloc;
		double _angular_veloc;
	} SteerCtrl;

	bool goto_pos( Position2DBridge& r_posbr, const Position2D& dest_pos , SteerCtrl& r_stc , double err= 0.5 );

	bool simple_collision_avoid( RangerBridge& r_rb, SteerCtrl& r_stc );

}
