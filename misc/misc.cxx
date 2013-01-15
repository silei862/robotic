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
#include <time.h>
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
		_rad = -2*PI + _rad;
	if( _rad < -1*PI )
		_rad = 2*PI + _rad;
	return _rad;
}

double SlamLab::delta2rad( double dx , double dy )
{
	double dd = sqrt( dx*dx + dy*dy );
	double rad = asin( dy/dd );
	if( dx < 0 )
	{
		if( rad >= 0 )
			rad = PI - rad;
		else
			rad = -1*PI - rad;
	}
	return rad;
}

double SlamLab::operator-( timeval_t& r_end , timeval_t& r_start )
{
	time_t dsec = r_end.tv_sec - r_start.tv_sec;
	suseconds_t dusec = r_end.tv_usec - r_start.tv_usec;
	// 借位：
	if( dusec < 0 )
	{
		dusec +=1000000;
		dsec--;
	}
	double delta_sec = double( dsec ) + double( dusec )/1000000.0;
	return delta_sec;
}

double SlamLab::average_random( double min , double max )
{
	int min_int = min*10000;
	int max_int = max*10000;
	int delta_int = max_int - min_int;
	int rand_int = random()%delta_int;
	return double(min_int+rand_int)/10000;
}

// 对数正太分布随机函数：
inline double lognormal( double x , double u , double sigma )
{
	return 1.0/(x*sqrt(2*PI)*sigma) * exp(-1*(log(x)-u)*(log(x)-u)/(2*sigma*sigma));
}

double SlamLab::lognormal_random( double u , double sigma ,double min , double max )
{
	double x;
	double d_scope;
	double y;
	do
	{
		x = average_random( min , max );
		y = lognormal( x , u , sigma );
		d_scope = average_random( 0 , lognormal( u , u , sigma ));
	}while( d_scope > y );
	return x;
}

double SlamLab::randnormal( double means , double sigma )
{
	double model = 4294967296.0;
	double multiplicator = 663608941.0;
	double temp_a, temp_b;
	static double seed = (double)time(NULL);

	seed *= multiplicator;
	seed -= (int)(seed/model) * model;
	temp_a = seed/model;
	seed *= multiplicator;
	seed -= (int)(seed/model) * model;
	temp_b = seed/model;
	double norm_randn = sqrt(-2*log(temp_a))*cos(2*PI*temp_b);
	double gauss_rand = means + norm_randn*sigma;
	return(gauss_rand);
}
