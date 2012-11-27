/*
 * =====================================================================================
 *
 *       Filename:  gridwin.h
 *
 *    Description:  提供对grid的一个窗口索引，可以加快操作速度  
 *
 *        Version:  1.0
 *        Created:  2012年10月25日 11时02分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */

#ifndef _GRIDWIN_H_
#define _GRIDWIN_H_

#include <iostream>
#include <stdint.h>
#include "coord.h"
#include "debug.h"

namespace SlamLab
{
	template <typename TP>
	class Grid;

	// 	 Class Name: GridWin
	// 	Description: 实现局部窗口与全局坐标的互换
	template <typename TP>
	class GridWin
	{
		public:
			GridWin( Grid<TP>* p_gd )		//指向被索引的网格
				:p_grid(p_gd),	
				x_base(0),y_base(0),
				width(p_grid->cell_rows()),
				height(p_grid->cell_cols()){ }

			GridWin( Grid<TP>* p_gd, 					//指向被索引网格
					uint32_t org_x, uint32_t org_y, 	//窗口在网格中的相对坐标
					uint32_t wd, uint32_t ht )			//窗口的大小
				:p_grid(p_gd),
				x_base(org_x),y_base(org_y),
				width(wd),height(ht){ }
		
			GridWin( Grid<TP>* p_gd, Point2D<uint32_t> org , uint32_t width , uint32_t height )
				:p_grid(p_gd),x_base(org._x),y_base(org._y),width(width),height(height){ }
		
			GridWin( Grid<TP>* p_gd,double org_x,double org_y,double width,double height)
				:p_grid(p_gd){ set_rect( org_x,org_y,width,height ); }

			GridWin( const GridWin<TP>& grid_win )
				:p_grid(grid_win.p_grid),x_base(grid_win.x_base),y_base(grid_win.y_base),
				width(grid_win.width),height(grid_win.height){ }

			virtual ~GridWin(){ }
	
		public:
			// 设置窗口在网格中的位置和大小：
			void set_region( uint32_t x, uint32_t y, uint32_t wd, uint32_t ht )
			{
				uint32_t grid_width = p_grid->cell_cols();
				uint32_t grid_height = p_grid->cell_rows();
				DBG_USER_INFO( "Int Size:",std::cout<<"W:"<<wd<<" H:"<<ht<<std::endl);
				// 原点限制，防止窗口原点超出网格
				x_base = ( x<grid_width )?x:0;
				y_base = ( y<grid_height )?y:0;
				// 边界限制，防止网格窗口超出网格
				width = ( grid_width<x_base+wd )?( grid_width-x_base ):wd;
				height = ( grid_height<y_base+ht )?( grid_height-y_base ):ht;
				DBG_USER_INFO( "Int Size 2:",std::cout<<"W:"<<width<<" H:"<<height<<std::endl);
			}

			// 使用坐标设置窗口
			// 防止窗口超出网格的情况发生
			void set_rect( double x ,double y, double wd, double ht )
			{
				Point2D<double> grid_org = p_grid->get_origin();
				double delta_x = x-grid_org._x;
				double delta_y = y-grid_org._y;
				double win_width,win_height,bx,by;
				// 窗口与网格相对位置判定：x方向
				if( delta_x< 0 )	// 窗口原点处于网格原点左边
				{
					bx = grid_org._x;	// 设置窗口新原点x坐标
					win_width = wd+delta_x;	//计算窗口实际宽度
					if( win_width<0 )	// 整个窗口处于网格之左
						win_width = 0;
				}
				else
				{
					bx = x;	
					if( delta_x >= p_grid->width() )	//窗口处于网格之右
						win_width = 0;
					else
					{
						double delta_width = p_grid->width() - delta_x - wd;
						if( delta_width < 0 )	// 部分窗口处于网格之右
							win_width = wd + delta_width; 	//计算实际宽度	
						else	// 在网格范围内
							win_width = wd;
					}
				}
				// 窗口与网格相对位置判定：y方向
				if( delta_y < 0 )	// 窗口原点位于网格原点下面
				{
					by = grid_org._y;		// 设置窗口新原点y坐标
					win_height = ht + delta_y;
					if( win_height < 0 )	// 整个窗口位于网格之下
						win_height = 0;
				}
				else
				{
					by = y;
					if( delta_y >= p_grid->height() )	//窗口处于网格之上
						win_height = 0;
					else
					{
						double delta_height = p_grid->height() - delta_y -ht;
						if( delta_height < 0 )	// 部分窗口处于网格上面
							win_height = ht + delta_height;
						else	// 在网格范围内
							win_height = ht;
					}
				}
				// 宽度或高度其中一个为零，窗口全零
				if( 0 == win_width || 0 == win_height )
				{
					win_width = 0;
					win_height = 0;
				}
				double cell_size = p_grid->cell_size();
				set_region( uint32_t( (bx - grid_org._x)/cell_size ),
							uint32_t( (by - grid_org._y)/cell_size ),
							uint32_t( win_width/cell_size ),
							uint32_t( win_height/cell_size ));
			}

			inline uint32_t cell_cols( ){ return width; }
			inline uint32_t cell_rows( ){ return height; }
			inline uint32_t get_x_base( ){ return x_base; }
			inline uint32_t get_y_base( ){ return y_base; }
			// 按照窗口索引获取元素
			inline TP& cell( uint32_t x_idx, uint32_t y_idx )
			{
				return (*p_grid)( x_base+ x_idx , y_base+y_idx );
			}
			// 按照索引获取元素坐标
			inline Point2D<double> cell_coord( uint32_t x_idx , uint32_t y_idx )
			{
				return p_grid->cell_coord( x_base + x_idx, y_base + y_idx );
			}
			// 操作符重载
			inline TP& operator()( uint32_t x_idx, uint32_t y_idx ){ return cell( x_idx , y_idx ); }
			inline void operator=( const GridWin& gridwin )
			{
				p_grid = gridwin.p_grid;
				x_base = gridwin.x_base;
				y_base = gridwin.y_base;
				width = gridwin.width;
				height = gridwin.height;
			}

		private:
			// 被索引的网格：
			Grid<TP>* p_grid;			
			// 窗口原点在网格中的横纵坐标：
			uint32_t x_base;			
			uint32_t y_base;
			// 窗口的宽度和高度：
			uint32_t width;
			uint32_t height;
	};
}

namespace std
{
	template < typename TP >
	std::ostream& operator<<( std::ostream& out, SlamLab::GridWin<TP>& gw )
	{
		out<<"[ GridWin Height:"<<gw.cell_rows()<<" Width:"<<gw.cell_cols()<<" ]"<<endl;
		for(uint32_t y=0;y<gw.cell_cols();y++)
		{
			for(uint32_t x=0;x<gw.cell_rows();x++)
				out<<gw(y,x)<<" ";
			out<<endl;
		}
		return out;
	}
}


#endif	//_GRIDWIN_H_
