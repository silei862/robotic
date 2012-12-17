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

#include <stdint.h>
#include <list>
#include <coord.h>
#include <grid.h>
#include <himmgrid.h>


namespace SlamLab
{
	typedef struct _dm_cell_t
	{
		double 			d;
		Point2D<size_t>	ob_pos;
	} dm_cell_t;

	class DistanceMap
	{
			
		public:
			DistanceMap( );
			DistanceMap( double width , double height , double cell_size , Point2D<double> org );
			// 使用一个概率网格地图的参数来生成距离网格地图:
			DistanceMap( HIMMGrid& r_hg );
			DistanceMap( const DistanceMap& dm );
		public:
			// 存取接口：
			inline dm_cell_t& operator()( size_t i , size_t j );
			inline dm_cell_t& operator()( double x , double y );
			inline dm_cell_t& operator()( Point2D<double> &pos );

		private:
			// 初始化函数
			inline void _int_map_data( );

		private:
			// 距离网格：
			Grid< dm_cell_t > 		_grid;
			// 距离最大值：
			double 					_max_distance;
	};
}

#endif //_DISTANCEGRID_H
