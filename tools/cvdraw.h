/*
 * =====================================================================================
 *
 *       Filename:  cvdraw.h
 *
 *    Description:  封装了OpenCV中的绘图功能
 *
 *        Version:  1.0
 *        Created:  2012年11月26日 16时13分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _CVDRAW_H_
#define _CVDRAW_H_

#include <cv.h>
#include <highgui.h>
#include "cvcanvas.h"

namespace SlamLab
{
	class CellDrawer
	{
		friend CVCanvas& operator<<( CVCanvas& , CellDrawer& );
		public:
			CellDrawer( cv::Scalar color = cv::Scalar(0,0,0) , int thickness = CV_FILLED )
				:_color( color ), _pt0(0,0),_pt1(1,1),_thickness(thickness){ }
		public:
			inline void set_color( cv::Scalar color ){ _color=color; }
			inline CellDrawer& operator()( size_t x0 , size_t y0 , size_t x1 , size_t y1 )
			{
				_pt0.x = x0; _pt0.y = y0;
				_pt1.x = x1; _pt1.y = y1;
			}
		private:
			cv::Scalar	_color;
			cv::Point	_pt0;
			cv::Point	_pt1;
			int			_thickness;
	};
	CVCanvas& operator<<( CVCanvas& , CellDrawer& );
}

#endif //_CVDRAW_H_
