/*
 * =====================================================================================
 *
 *       Filename:  cvdraw.cxx
 *
 *    Description:  封装OpenCV中的绘图功能
 *
 *        Version:  1.0
 *        Created:  2012年11月26日 16时13分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "cvdraw.h"

using namespace cv;
using namespace SlamLab;

/*CVCanvas& SlamLab::operator<<( CVCanvas& r_canvas , CVPoint& r_point )
{
	// To do:
}*/

CVCanvas& SlamLab::operator<<( CVCanvas& r_canvas , CellDrawer& r_cd )
{
	Mat& canvas = r_canvas.canvas();
	rectangle( canvas, r_cd._pt0,r_cd._pt1,r_cd._color ,r_cd._thickness );
}
