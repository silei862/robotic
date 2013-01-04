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


namespace SlamLab
{
	const double PI=3.1415926535;
	
	inline double R2D( double rad ){ return rad*180/3.1415926535; }
	
	inline double rad_standardize( double rad );
	// 角度限制：
	double rad_hold( double rad );
	// 计算坐标差计算弧度：
	double delta2rad( double dx , double dy );	
}

#endif //_MISC_H_
