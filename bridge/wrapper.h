/*
 * =====================================================================================
 *
 *       Filename:  wrapper.h
 *
 *    Description:  传感器的包装类
 *
 *        Version:  1.0
 *        Created:  2012年11月14日 21时23分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <stdint.h>
#include <vector>
#include <coord.h>
#include <debug.h>

namespace SlamLab
{
	enum _WRAPPER_MODEL
	{
		MODEL_UNKNOW	= 0,
		MODEL_RANGER	= 1,
		MODEL_POSE		= 2,
		MODEL_ID_USER 	=1000,
	};
// 包装类的基类：
	class Wrapper
	{
		public:
			Wrapper( uint32_t model=MODEL_UNKNOW )
				:_model(model){ }
			virtual ~Wrapper(){ };
		public:
			uint32_t model(){ return _model; }
		private:
			uint32_t _model;
	};

//	测距仪的接口：
	class IRangerWrapper : public Wrapper
	{
		public:
			IRangerWrapper( uint32_t model=MODEL_RANGER )
				:Wrapper( model ){ }
			virtual ~IRangerWrapper(){ };
		public:
			virtual double get_data( size_t index ){};
			virtual size_t ranger_num(){};
			virtual Pose3D& get_pose( size_t index ){};
	};

//  机器人姿态接口
	class IPosition2DWrapper : public Wrapper
	{
		public:
			IPosition2DWrapper( uint32_t model=MODEL_POSE )
				:Wrapper( model ){ }
			virtual ~IPosition2DWrapper(){ };
		public:
			virtual void set_speed( double a_speed, double yaw_speed ){};
			virtual double get_x_pos(){};
			virtual double get_y_pos(){};
			virtual double get_yaw(){};
	};
}

#endif 	//_WRAPPER_H_
