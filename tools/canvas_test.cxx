/*
 * =====================================================================================
 *
 *       Filename:  canvas_test.cxx
 *
 *    Description:  测试图像显示
 *
 *        Version:  1.0
 *        Created:  2012年11月25日 22时09分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "cvcanvas.h"
#include "cvdraw.h"

using namespace SlamLab;
using namespace cv;

int main()
{
	CVCanvas canvas(500,500,"测试");
	CellDrawer rect( Scalar(120,0,0) );
	CVImageFile img_file("/home/robot/1.jpg");
	//img_file("/home/robot/1.png")>>canvas;
	canvas.refresh();
	canvas<<rect( 10,10,55,55 );
	canvas.refresh();
	cv::waitKey();
	return EXIT_SUCCESS;
}
