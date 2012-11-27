/*
 * =====================================================================================
 *
 *       Filename:  cvcanvas.cxx
 *
 *    Description:  OpenCV绘图功能的封装实现
 *
 *        Version:  1.0
 *        Created:  2012年11月26日 13时59分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <debug.h>
#include "cvcanvas.h"

using namespace SlamLab;
using namespace cv;
using namespace std;
// ------------------------ CVCanvas -------------------------------------------



CVCanvas::CVCanvas( size_t width , size_t height , string title )
		:_background_color( 255,255,255 ),
		 _title( title ),
		 _canvas( height, width , CV_8UC3, Scalar(255,255,255) )
{
	namedWindow( _title , CV_WINDOW_AUTOSIZE );
	refresh();
}

CVCanvas::~CVCanvas()
{
	destroyWindow( _title );
}

// ------------------------ CVImageFile ----------------------------------------
CVImageFile& CVImageFile::operator()( const char* file_name )
{
	_file_name = string( file_name );
	return (*this);
}

CVImageFile& CVImageFile::operator()( const string& file_name )
{
	_file_name = file_name;
	return (*this);
}

void CVImageFile::operator>>( CVCanvas& r_canvas )
{
	Mat img = imread( _file_name );
	if( img.empty() )
		throw( Exception("Image Load failed!",EXID_FILE_ERROR,__FILE__,__LINE__) );
	r_canvas.canvas() = img.clone();
}

void CVImageFile::operator<<( CVCanvas& canvas )
{
	//To do:	
}
