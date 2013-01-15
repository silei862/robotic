/*
 * =====================================================================================
 *
 *       Filename:  misc.h
 *
 *    Description:  一些辅助的代码
 *
 *        Version:  1.0
 *        Created:  2012年10月15日 16时33分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */
#ifndef _MISC_H_
#define _MISC_H_

#include <sys/time.h>

namespace SlamLab
{
	typedef struct timeval timeval_t;
	const double PI=3.1415926535;
	
	inline double R2D( double rad ){ return rad*180/3.1415926535; }
	
	double rad_standardize( double rad );
	// 角度限制：
	double rad_hold( double rad );
	// 计算坐标差计算弧度：
	double delta2rad( double dx , double dy );	
	// 计算时间差：
	double operator-( timeval_t& r_start , timeval_t& r_end ); 
	// 产生min~max之间的均匀分布的随机数
	double average_random( double min , double max );
	// 产生正太分布随机函数
	double randnormal( double means , double sigma );
	// 产生min与max之间符合对数正态分布的随机函数
	double lognormal_random( double u , double sigma, double min , double max );
}

#endif //_MISC_H_
