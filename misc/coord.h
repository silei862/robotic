/*
 * =====================================================================================
 *
 *       Filename:  coord.h
 *
 *    Description:  坐标相关的类
 *
 *        Version:  1.0
 *        Created:  2012年10月15日 16时36分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */

#ifndef _COORD_H_
#define _COORD_H_
#include <iostream>

namespace SlamLab
{
	// 	 Class Name：Point2D
	// 	Description：描述二维直角坐标系的点
	template <typename TP >
	class Point2D
	{
		public:
			Point2D()
				:_x(0),_y(0){ }
			Point2D( TP x, TP y )
				:_x(x),_y(y){ }
			Point2D( const Point2D<TP>& pt )
				:_x(pt._x),_y(pt._y){ }
		public:
			inline void operator= ( const Point2D<TP>& pt )
			{
				_x = pt._x;
				_y = pt._y;
			}
			inline bool operator==( const Point2D<TP>& pt ){ return pt._x==_x && pt._y==_y; }
			inline bool operator!=( const Point2D<TP>& pt ){ return pt._x!=_x || pt._y!=_y; }
		public:
			TP _x;	//横坐标
			TP _y;	//纵坐标
	};

	//	 Class Name: Point3D
	//	Description: 描述三维直角坐标系的点

	template < typename TP >
	class Point3D
	{
		public:
			Point3D()
				:_x(0),_y(0),_z(0){ }
			Point3D( TP x, TP y, TP z )
				:_x(x),_y(y),_z(z){ }
			Point3D( const Point3D<TP>& pt )
				:_x(pt._x),_y(pt._y),_z(pt._z){ }
		public:
			inline void operator=( const Point3D<TP>& pt )
			{
				_x = pt._x;
				_y = pt._y;
				_z = pt._z;
			}
			inline bool operator==( const Point3D<TP>& pt){ return (pt._x==_x)&&(pt._y==_y)&&(pt._z==_z); }
			inline bool operator!=( const Point3D<TP>& pt){ return (pt._x!=_x)||(pt._y!=_y)||(pt._z!=_z); }
		public:
			TP _x;
			TP _y;
			TP _z;
	};

	class Pose3D;
	std::ostream& operator<<( std::ostream& os,const Pose3D& pose );

	//	 Class Name:	Pose3D
	//	Description:	用于描述空间中的刚体姿态

	class Pose3D
	{
		friend std::ostream& operator<<( std::ostream& os, const Pose3D& pose );
		public:
			Pose3D( const Point3D<double>& pos=Point3D<double>(0.0,0.0,0.0),
					double rx=0.0,
					double ry=0.0,
					double rz=0.0 )
				:_pos(pos),_rx(rx),_ry(ry),_rz(rz){ }
			Pose3D( const Pose3D& pose )
				:_pos(pose._pos),_rx(pose._rx),_ry(pose._ry),_rz(pose._rz){ }

		public:
			inline void operator=( const Pose3D pose )
			{
				_pos=pose._pos;
				_rx=pose._rx;
				_ry=pose._ry;
				_rz=pose._rz;
			}

		public:
			Point3D<double> _pos;
			double _rx;
			double _ry;
			double _rz;
	};
}

#endif	//_COORD_H_
