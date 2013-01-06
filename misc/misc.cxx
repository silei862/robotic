/*
 * =====================================================================================
 *
 *       Filename:  misc.cxx
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月21日 19时39分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <cmath>
#include "misc.h"
using namespace SlamLab;


double SlamLab::rad_standardize( double rad )
{
	double _rad = fmod( rad , 2*PI );
	if( _rad <0 )
		_rad+=2*PI;
	return _rad;
}

double SlamLab::rad_hold( double rad )
{
	double _rad = fmod( rad , 2*PI );
	if( _rad > PI )
		_rad = 2*PI - _rad;
	if( _rad < -1*PI )
		_rad = 2*PI + _rad;
	return _rad;
}

double SlamLab::delta2rad( double dx , double dy )
{
	double dd = sqrt( dx*dx + dy*dy );
	double rad = asin( dy/dd );
	if( dx < 0 )
		if( rad >= 0 )
			rad = PI - rad;
		else
			rad = -1*PI - rad;
	return rad;
}
