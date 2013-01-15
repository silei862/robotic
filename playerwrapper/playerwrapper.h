/*
 * =====================================================================================
 *
 *       Filename:  playerwrapper.h
 *
 *    Description:  player平台各个传感器接口的包装
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 10时26分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _PLAYERWRAPPER_H_
#define _PLAYERWRAPPER_H_

#include <coord.h>
#include <wrapper.h>
#include <string>
#include <libplayerc++/playerc++.h>
#include <stdint.h>
#include <stdlib.h>

namespace SlamLab
{
// ------------------ Player Ranger Wrapper ------------------------
	class RangerPlayer : public IRangerWrapper
	{
		public:
			RangerPlayer( PlayerCc::RangerProxy& ranger, 
					uint32_t model=MODEL_RANGER )
				:IRangerWrapper( model ),r_ranger( ranger ){ }
			
			virtual ~RangerPlayer(){ };
		public:
			virtual double get_data( size_t index );
			virtual size_t ranger_num();
			virtual Pose3D& get_pose( size_t index );
		private:
			PlayerCc::RangerProxy& r_ranger;
			Pose3D _pose;
	};

// ------------------- 带正太分布噪声的距离传感器包装 ----------------
	class RangerNoisePlayer : public IRangerWrapper
	{
		public:
			RangerNoisePlayer( PlayerCc::RangerProxy& ranger,
					double portion,
					uint32_t model = MODEL_RANGER )
				:IRangerWrapper( model ),_portion(portion),r_ranger( ranger ){ }
			virtual ~RangerNoisePlayer(){ };
		public:
			virtual double get_data( size_t index );
			virtual size_t ranger_num();
			virtual Pose3D& get_pose( size_t index );
		private:
			PlayerCc::RangerProxy& r_ranger;
			double _portion;
			Pose3D _pose;
	};

// ------------------ Player Sonar Wrapper -----------------------
	class SonarPlayer : public IRangerWrapper
	{
		public:
			SonarPlayer( PlayerCc::SonarProxy& sonar,
					uint32_t model=MODEL_RANGER )
				:IRangerWrapper( model ), r_sonar( sonar ){ }
			virtual ~SonarPlayer(){ };
		public:
			virtual double get_data( size_t index );
			virtual size_t ranger_num();
			virtual Pose3D& get_pose( size_t index );
		private:
			PlayerCc::SonarProxy& r_sonar;
			Pose3D _pose;
	};

// ------------------ Player Position2D Wrapper ------------------
	class Position2DPlayer : public IPosition2DWrapper
	{
		public:
			Position2DPlayer( PlayerCc::Position2dProxy& pos2d,
					uint32_t model=MODEL_POSE )
				:IPosition2DWrapper( model ),r_pos2d( pos2d ){ }
			virtual ~Position2DPlayer(){ };
		public:
			virtual void set_speed( double a_speed, double yaw_speed );
			virtual double get_x_pos();
			virtual double get_y_pos();
			virtual double get_yaw();
		private:
			PlayerCc::Position2dProxy& r_pos2d;
	};

// ----------------- 带噪声的位置位置机构 -------------------------
	class Position2DNoisePlayer : public IPosition2DWrapper
	{
		public:
			Position2DNoisePlayer( PlayerCc::Position2dProxy& pos2d,
					double sg_aspeed,
					double sg_yspeed,
					double sg_xy,
					double sg_yaw,
					uint32_t model=MODEL_POSE )
				:IPosition2DWrapper( model ),
				_sigma_aspeed( sg_aspeed ),
				_sigma_yspeed( sg_yspeed ),
				_sigma_xy( sg_xy ),
				_sigma_yaw( sg_yaw ),
				r_pos2d( pos2d ){ }
			virtual ~Position2DNoisePlayer(){ };
		public:
			virtual void set_speed( double a_speed, double yaw_speed );
			virtual double get_x_pos();
			virtual double get_y_pos();
			virtual double get_yaw();
		private:
			PlayerCc::Position2dProxy& r_pos2d;
			double _sigma_aspeed;
			double _sigma_yspeed;
			double _sigma_xy;
			double _sigma_yaw;

	};

}

#endif // _PLAYERWRAPPER_H_
