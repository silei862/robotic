/*
 * =====================================================================================
 *
 *       Filename:  grid.h
 *
 *    Description:  网格类，网格地图的基础
 *
 *        Version:  1.0
 *        Created:  2012年10月17日 10时30分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */

#ifndef _GRID_H_
#define _GRID_H_

#include <stdint.h>
#include <stdlib.h>
#include "array2d.h"
#include "coord.h"
#include "gridwin.h"
#include "debug.h"

namespace SlamLab
{
	// 	Class Name:	Grid
	// Description: 地图网格化类
	template< typename TP>
	class Grid
	{
		// 默认参数：
		enum _PARAMENTER
		{
			DEFAULT_GRID_WIDTH = 10,
			DEFAULT_GRID_HEIGHT = 10
		};
		const static double DEFAULT_CELL_SIZE = 0.5;
	
		public:
			Grid()
				:_cell_size( DEFAULT_CELL_SIZE ),
				_grid_data( DEFAULT_GRID_WIDTH,DEFAULT_GRID_HEIGHT ),
				_origin(0,0), 
				_gridwin(this){ }
		
			// -----参数说明-----
			// width,height：网格化地图的实际尺寸
			// _cell_size：每个单元格的实际边长
			// org：网格原点（0,0）在实际坐标中的位置
			Grid( double width, double height, double cell_size, Point2D<double> org )
				:_cell_size(cell_size),
				_grid_data(width/_cell_size+1,height/_cell_size+1),
				_origin( org ), 
				_gridwin(this){ }

			Grid( size_t cols , size_t rows , double cell_size , Point2D<double> org )
				:_cell_size( cell_size ),
				_grid_data( uint32_t( cols ) , uint32_t( rows ) ),
				_origin( org ) , 
				_gridwin( this ){ }
		
			Grid( const Grid<TP>& grid )
				:_cell_size( grid._cell_size ),
				_grid_data( grid._grid_data ),
				_origin( grid._origin ),
				_gridwin(this){ }

			virtual ~Grid() { }
	
		public:
			// 网格基本属性的获取与设置
			// 设置获取原点
			inline void set_origin( const Point2D<double>& pt ) { _origin = pt; }
			inline const Point2D<double>& get_origin() const { return _origin; }
			inline const Point2D<double>& origin() const { return _origin; }
			// 获取、设置网格单元所代表的尺寸
			inline double cell_size(){ return _cell_size; }
			// 调整网格属性
			void set_attr( double width, double height, double cell_size, Point2D<double> org )
			{ 
				_cell_size = cell_size;
				_origin = org;
				_grid_data.resize( width/_cell_size+1,height/_cell_size+1 ); 
			}
			void set_attr( size_t cols , size_t rows , double cell_size, Point2D<double> org )
			{
				_cell_size = cell_size;
				_origin = org;
				_grid_data.resize( uint32_t(cols) , uint32_t(rows) );
			}
			// 获取网格长、宽：
			inline double width(){ return _grid_data.col_size()*_cell_size; }
			inline double height(){ return _grid_data.row_size()*_cell_size; }
			// 获取网格横纵单元数：
			inline uint32_t cell_cols(){ return _grid_data.col_size(); }
			inline uint32_t cell_rows(){ return _grid_data.row_size(); }
			inline uint32_t cols() { return _grid_data.col_size(); }
			inline uint32_t rows() { return _grid_data.row_size(); }
			// 获取网格坐标
			inline Point2D<double> cell_coord( uint32_t x, uint32_t y ) const
			{
				Point2D<double> center;
				center._x = _origin._x + _cell_size*double(x)+_cell_size/2;
				center._y = _origin._y + _cell_size*double(y)+_cell_size/2;
				return center;
			}
			// 将坐标转换为行列序号
			inline Point2D<uint32_t> pos2sq( double x , double y ) const
			{
				Point2D<uint32_t> sq;
				sq._x = uint32_t((x-_origin._x)/_cell_size);
				sq._y = uint32_t((y-_origin._y)/_cell_size);
				return sq;
			}

			inline Point2D<uint32_t> pos2sq( Point2D<double> pos ) const
			{
				return pos2sq( pos._x , pos._y );
			}

			// 越界判断
			bool in(double x,double y)
			{
				uint32_t ix=uint32_t((x-_origin._x)/_cell_size);
				uint32_t iy=uint32_t((y-_origin._y)/_cell_size);
				return ( (ix>=0)&&(iy>=0)&&(ix<_grid_data.col_size())&&(iy<_grid_data.row_size()) );
			}
			bool in( int32_t x, int32_t y)
			{
				return ( x>=0 )&&( y>=0)&&(x<_grid_data.col_size())&&(y<_grid_data.row_size());
			}
			// 设置获取网格数据
			inline Array2D<TP>& grid_data(){ return _grid_data; }
			// 注：替换网格数据后，网格尺寸将发生变化
			inline void set_grid_data(Array2D<TP>& data){ _grid_data = data; }
			// 整体赋值
			inline void set_all_val( const TP& val ){ _grid_data.set_all_val( val ); }
			// 获取网格窗口
			inline GridWin<TP>& get_win(){ return _gridwin; }
			// 指定中心坐标和半径，获取网格窗口
			inline GridWin<TP>& get_win( double cx, double cy, double radius )
			{
				_gridwin.set_rect( cx-radius,cy-radius,2*radius,2*radius );
				return _gridwin;
			}
			// 直接指定原点坐标和高度、宽度，获取网格窗口
			inline GridWin<TP>& get_win( double x, double y, double wd ,double ht )
			{
				_gridwin.set_rect( x-wd/2,y-ht/2,wd,ht );
				return _gridwin;
			}

			// 操作符：
			void operator=( const Grid<TP>& grid )
			{
				_cell_size = grid._cell_size;
				_origin = grid._origin;
				_grid_data = grid._grid_data;
			}
			TP& operator()( uint32_t x, uint32_t y ){ return _grid_data( x,y ); }
			TP& operator()( size_t i , size_t j ){ return _grid_data( uint32_t( i ) , uint32_t( j ) ); }
			TP& operator()( double x, double y ){ return _grid_data( uint32_t((x-_origin._x)/_cell_size), uint32_t((y-_origin._y)/_cell_size)); }
		
		private:
			double _cell_size;
			Array2D<TP> _grid_data;
			Point2D<double> _origin;
			GridWin<TP> _gridwin;
	};
}

#endif	//_GRID_H_
