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
	// 类型别名：
	typedef Point2D<double>		float_pos_t;	
	typedef Point2D<size_t>		grid_pos_t;

	typedef struct _dm_cell_t
	{
		double 		_d;
		grid_pos_t	_ob_pos;
	} dm_cell_t;

	// 距离网格数据类型：
	typedef Grid< dm_cell_t >	dm_data_t;
	typedef GridWin< dm_cell_t>	dm_win_t;

	class DistanceMap
	{

		public:
			DistanceMap( );
			DistanceMap( double width , double height , double cell_size , Point2D<double> org );
			// 使用一个概率网格地图的参数来生成距离网格地图:
			DistanceMap( HIMMGrid& r_hg );
			DistanceMap( const DistanceMap& dm );
		public:
			// 属性获取：
			size_t cols();
			size_t rows();
			double width();;
			double height();
			double cell_size();
			float_pos_t origin();
			// 直接获取距离网格引用
			dm_data_t& get_dm_grid();
			// 获取窗口
			// 需要指定窗口原点全局坐标org , 高width、宽height
			dm_win_t& get_win( float_pos_t org , double width, double height );
			// 指定窗口边长一半radius
			dm_win_t& get_win( float_pos_t org , double radius );
			// 转换函数：
			grid_pos_t pos2sq( float_pos_t pos );
			grid_pos_t pos2sq( double x , double y );
			// 越界判断：
			bool in( double x , double y );
			bool in( size_t i , size_t j );
			// 坐标存取接口：
			dm_cell_t& operator()( size_t i , size_t j );
			dm_cell_t& operator()( grid_pos_t& pos );
			dm_cell_t& operator()( double x , double y );
			dm_cell_t& operator()( float_pos_t& pos );
			// 障碍格除：
			void clear_cell( size_t i , size_t j );
			void clear_cell( double x , double y );
			void clear_cell( grid_pos_t& r_pos );
			void clear_cell( float_pos_t& r_pos );

		private:
			// 初始化函数
			inline void _init_map_data( );

		private:
			// 距离网格：
			dm_data_t	_grid;
			// 距离最大值：
			double 		_max_distance;
	};
}

namespace std
{
	std::ostream& operator<<( std::ostream& r_os , SlamLab::DistanceMap& r_dm );
}

#endif //_DISTANCEGRID_H
