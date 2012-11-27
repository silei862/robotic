/*
 * =====================================================================================
 *
 *       Filename:  bridge.h
 *
 *    Description:  各类传感器、执行机构的桥接接口
 *
 *        Version:  1.0
 *        Created:  2012年11月16日 09时23分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#include <iostream>
#include <wrapper.h>
namespace SlamLab
{

class Bridge
{
	public:
		Bridge( Wrapper* p_wp )
			:p_wrapper( p_wp ){ }
		virtual ~Bridge(){ };
	public:
		inline Wrapper* get_wrapper(){ return p_wrapper; }
		inline void set_wrapper( Wrapper* p_wp ){ p_wrapper = p_wp; }
	private:
		Wrapper* p_wrapper;
};

//	距离传感器桥接类
	class RangerBridge : public Bridge
	{
		public:
			RangerBridge( Wrapper* p_rg )
				:Bridge(p_rg){ }
		public:
			Pose3D& get_pose( size_t index );
			size_t ranger_num();
			double operator[]( size_t index );
	};

//	获取姿态的桥接类
	class Position2DBridge : public Bridge
	{
		public:
			Position2DBridge( Wrapper* p_ps )
				:Bridge(p_ps){ }
		public:
			void set_speed( double a_speed , double yaw_speed );
			double get_x_pos();
			double get_y_pos();
			double get_yaw();
	};
}

namespace std
{
	std::ostream& operator<<( std::ostream& os , SlamLab::RangerBridge &rb );
	std::ostream& operator<<( std::ostream& os , SlamLab::Position2DBridge &pb );
}

#endif	//_BRIdGE_H_
