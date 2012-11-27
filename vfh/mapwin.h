/*
 * =====================================================================================
 *
 *       Filename:  mapwin.h
 *
 *    Description:  用于VFH算法的专用地图窗口
 *
 *        Version:  1.0
 *        Created:  2012年11月24日 09时36分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _MAPWIN_H_
#define _MAPWIN_H_

#include <grid.h>
#include <types.h>

namespace SlamLab
{
	template< typename TP >
		class TMapWin
		{
			public:
				TMapWin( double x , double y, double r, Grid<TP>* p_g , TP f_val)
				{
					p_grid = p_g;
					Position2D grid_pos = p_g->get_origin();
					_base_x = int32_t(( x - r - grid_pos._x )/p_g->cell_size());
					_base_y = int32_t(( y - r - grid_pos._y )/p_g->cell_size());
					_width = int32_t(2*r/p_g->cell_size());
					_height = int32_t(2*r/p_g->cell_size());
					fake_val = f_val;
				}

			public:
				size_t width(){ return size_t(_width); }
				size_t height(){ return size_t(_height); }
				Position2D cell_coord( int32_t x,int32_t y )
				{
					Position2D grid_pos = p_grid->get_origin();
					double cellsize = p_grid->cell_size();
					double cx = grid_pos._x + double(x+_base_x)*cellsize+cellsize/2;
					double cy = grid_pos._y + double(y+_base_y)*cellsize+cellsize/2;
					return( Position2D( cx, cy) );
				}

				void set_grid( Grid<TP>* p_g ){ p_grid = p_g; }
				void set_size( int32_t width, int32_t height)
				{
					_width = width;
					_height = height;
				}
				
				void set_fakevalue( TP f_val ){ fake_val=f_val; }

				TP& get_ref( int32_t x ,int32_t y )
				{
					int32_t gx=_base_x+x;
					int32_t gy=_base_y+y;
					if( p_grid->in( gx , gy ) )
						return (*p_grid)( uint32_t(gx),uint32_t(gy) );
					else
						return fake_val; 
				}

				TP& operator()( int32_t x , int32_t y ) { return get_ref( x , y ); }

			private:
				Grid<TP>*	p_grid;
				int32_t		_base_x;
				int32_t		_base_y;
				int32_t		_width;
				int32_t		_height;
				TP			fake_val;
		};
}


#endif //_MAPWIN_H_
