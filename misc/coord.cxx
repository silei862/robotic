/*
 * =====================================================================================
 *
 *       Filename:  coord.cxx
 *
 *    Description:  坐标系描述的实现文件
 *
 *        Version:  1.0
 *        Created:  2012年11月15日 20时53分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "coord.h"
using namespace SlamLab;

std::ostream& operator <<( std::ostream& os , const Pose3D& pose )
{
	os<<"Position["<<pose._pos._x<<","<<pose._pos._y<<","<<pose._pos._z<<"]&";
	os<<"Rotation["<<pose._rx<<","<<pose._ry<<","<<pose._rz<<"]";
	return os;
}
