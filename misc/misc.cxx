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
