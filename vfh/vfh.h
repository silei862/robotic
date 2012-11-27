/*
 * =====================================================================================
 *
 *       Filename:  vfh.h
 *
 *    Description:  Vector Field Histogram实现
 *
 *        Version:  1.0
 *        Created:  2012年11月21日 11时19分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _VFH_H_
#define _VFH_H_

#include <coord.h>
#include "vfh_types.h"
#include <himmgrid.h>
#include <vector>

// ---------------------------------- PHBuilder --------------------------------------------
namespace SlamLab
{
	class PHBuilder
	{
		public:
			static const double PI = 3.1415926535;
			static const double BB = 0.125;
			static const uint8_t bound_val = 15;
		public:
			PHBuilder( double safe_dist,double robot_size,double b=BB )
				:_center_pos( Point2D<double>( 0.0,0.0 ) ),
				_radius(5.0),
				p_map( NULL ),
				p_histo( NULL ),
				_safe_dist( safe_dist ),
				_robot_size( robot_size ),
				B(b){ }

			virtual ~PHBuilder(){ }
		
		public:
			// 计算参数设置
			void set_center( double x, double y ){ _center_pos = Point2D<double>( x,y ); }
			void set_radius( double radius){ _radius=radius; }
			void set_map( HIMMGrid* p_m ){ p_map = p_m; }
			void set_histogram( PolarHist* p_h){ p_histo = p_h; }
			//主构建函数
			void build();
			// 参数的接受，返回引用，和>>配合，方便使用
			PHBuilder& operator()( double x , double y );
			PHBuilder& operator()( double x, double y, double rd );
			PolarHist& operator>>( PolarHist& r_ph );
		
		private:
			
		private:
			Position2D	_center_pos;
			double		_radius;
			HIMMGrid*	p_map;
			PolarHist*	p_histo;
		// 其他参数
		private:
			double		_safe_dist;
			double		_robot_size;
			double		A;
			double		B;
	};


	PHBuilder& operator>>( HIMMGrid& r_map , PHBuilder& r_phb );
}

// ------------------------------------ BinPHBuilder --------------------------------------
namespace SlamLab
{
	class BinPHBuilder
	{
			static const int32_t DEF_RESOL=4;

		public:
			BinPHBuilder( int32_t resolution=DEF_RESOL )
				:_resolution( resolution ),
				p_polarhist( NULL ),p_binhist( NULL ),				
				_threshold_high( 0 ),
				_threshold_low( 0 ){ }
		
		public:
			// 成员访问方法：
			void set_polarhist( PolarHist* p_ph ){ p_polarhist = p_ph; }
			void set_binhist( PolarBinHist* p_bph ){ p_binhist = p_bph; }
			// 操作符号重载:
			PolarBinHist& operator>>( PolarBinHist& r_bph );
			BinPHBuilder& operator()( int32_t resol );
			// 构建二值直方图：
			void build();

		protected:
			// 计算上下阈值：
			void init_threshold();
				
		private:
			PolarHist* 		p_polarhist;
			PolarBinHist*	p_binhist;
			int32_t			_resolution;
			int32_t			_threshold_high;
			int32_t			_threshold_low;
	};
	BinPHBuilder& operator>>( PolarHist& r_ph , BinPHBuilder& r_binbuild );
}

namespace std
{
	ostream& operator<<( ostream& os , SlamLab::PolarHist& r_ph );
	ostream& operator<<( ostream& os , SlamLab::PolarBinHist& r_pbh );
}

#endif //_VFH_H_
