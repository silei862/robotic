/*
 * =====================================================================================
 *
 *       Filename:  cvcanvas.h
 *
 *    Description:  包装了opencv的绘图功能
 *
 *        Version:  1.0
 *        Created:  2012年11月26日 13时57分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _CVCANVAS_H_
#define _CVCANVAS_H_

#include <cv.h>
#include <highgui.h>
#include <string>

namespace SlamLab
{
	// 画布封装基本绘图功能：
	class CVCanvas
	{
		static const size_t WIDTH = 100;
		static const size_t HEIGHT = 100;
		public:
			CVCanvas( size_t width = WIDTH ,
					  size_t height = HEIGHT , 
					  std::string title = "Noname" );
			virtual ~CVCanvas();
		
		public:
			void set_title( std::string title ){ _title = title; }
			void refresh(){ cv::imshow( _title, _canvas ); }
			void close() { cv::destroyWindow(_title); }
			// 返回引用，方便后续类使用
			cv::Mat& canvas(){ return _canvas; }

			// 设置基本画布功能
			void set_background_color( cv::Scalar& color );
			void zoom( double scale );
		private:
			std::string _title;
			cv::Mat 	_canvas;
			cv::Scalar	_background_color;

	};
	
	// 图像文件接口
	class CVImageFile
	{
		public:
			CVImageFile( const char* file_name )
				:_file_name( file_name ){ }
			CVImageFile( std::string& file_name )
				:_file_name( file_name ){ }

			CVImageFile& operator()( const char* file_name );
			CVImageFile& operator()( const std::string& file_name );	
			void operator<<( CVCanvas& r_canvas );
			void operator>>( CVCanvas& r_canvas );
		private:
			std::string _file_name;
	};
	
}

#endif	//_CVCANVAS_H_
