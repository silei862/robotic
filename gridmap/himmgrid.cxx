/*
 * =====================================================================================
 *
 *       Filename:  himmgrid.cxx
 *
 *    Description:  实现文件
 *
 *        Version:  1.0
 *        Created:  2012年11月20日 20时24分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include "himmgrid.h"

using namespace SlamLab;

/* CVCanvas& SlamLab::operator<<( CVCanvas& r_canvas , HIMMGrid& hg )
{
	CellDrawer cell_draw;
	for( uint32_t y = 0; y< hg.cell_rows(); y++ )
		for( uint32_t x =0; x< hg.cell_cols(); x++ )
		{
			uint8_t cell_val = hg( x,y );
			int dc = (255 - int( cell_val )*10 );
			cell_draw.set_color(cv::Scalar(dc,dc,dc));
			int dy = int( hg.cell_cols() - y)*4;
			int dx = int(x*4);
			r_canvas<<cell_draw(dx,dy,dx+3,dy+3 );
		}
	return r_canvas;
}*/

std::ostream& std::operator<<( std::ostream& os , SlamLab::HIMMGrid& hg )
{
	os<<"HIMM Grid Map[ height:"<<hg.height()<<" width:"<<hg.width()<<" cell size:"<<hg.cell_size()<<" ]"<<std::endl;
	os<<"Grid[ row:"<<int(hg.cell_rows())<<" col:"<<int(hg.cell_cols())<<" ]"<<std::endl;
	for( int y =hg.cell_rows()-1;y>=0; y--)
	{
		for( int x = 0; x<hg.cell_cols();x++ )
		{
			if( 0==x || (hg.cell_cols()-1)==x )
			{
				os<<"| ";
				continue;
			}
			if( 0==y || (hg.cell_rows()-1)==y )
			{
				os<<"__";
				continue;
			}
			if( 0==hg(uint32_t(x),uint32_t(y)) )
				os<<" "<<" ";
			else
				os<<char(hg(uint32_t(x),uint32_t(y))+'A'-1)<<" ";
		}
		os<<std::endl;
	}
	return os;
}

std::ostream& std::operator<<( std::ostream& os , SlamLab::HIMMWin& hw )
{
	os<<"HIMM Grid Window[ height:"<<hw.cell_rows()<<" width:"<<hw.cell_cols();
	os<<" x_base:"<<hw.get_x_base()<<" y_base:"<<hw.get_y_base()<<" ]"<<std::endl;
	for( int y =hw.cell_rows()-1; y>=0 ; y--)
	{
		for( int x=0; x<hw.cell_cols(); x++ )
		{
			if( x==0 ||x==(hw.cell_cols()-1) )
			{
				os<<"|"<<" ";
				continue;
			}
			if( 0==y||(hw.cell_rows()-1) ==y)
			{
				os<<"__";
				continue;
			}
			if( 0==hw(x,y) )
			{
				os<<" "<<" ";
				continue;
			}
			else
			{
				os<<char(hw(x,y)+'A'-1)<<" ";
				continue;
			}
		}
		os<<std::endl;
	}
	return os;
}
