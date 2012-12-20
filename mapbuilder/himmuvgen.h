/*
 * =====================================================================================
 *
 *       Filename:  himmuvgen.h
 *
 *    Description:  HIMM更新向量生成及地图维护器
 *
 *        Version:  1.0
 *        Created:  2012年12月14日 09时02分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _HIMMUVGEN_H_
#define _HIMMUVGEN_H_

#include "updatevector.h"
#include <bridge.h>
#include <array2d.h>
#include <himmgrid.h>

namespace SlamLab
{

	class HIMMUVGen
	{
		// ---------------------- 相关常量 -------------------------------
		enum _CONST
		{
			DEF_TPL_SIZE	= 3,
			TPL_NUM			= DEF_TPL_SIZE*DEF_TPL_SIZE,
			DEF_INC			= 3,
			DEF_DEC			= 1,
		};
		const static double tpl_val[ TPL_NUM ];	
		const static double def_min_reading;
		const static double def_max_reading;
		// ---------------------- 对外接口 --------------------------------
		public:
			HIMMUVGen( int inc = DEF_INC,
					   int dec = DEF_DEC, 
					   double max_reading = def_max_reading );
			~HIMMUVGen(){ }
		public:
			HIMMUVGen& operator()( Position2DBridge& r_pos2d , HIMMGrid& r_hg );
			uvectors_t& operator>>( uvectors_t& r_uv );
			void set_pos2d( Position2DBridge& r_pos2d );
			void set_ranger( RangerBridge& r_ranger );
			void set_uvector( uvectors_t& r_uv );
			void set_himmgrid( HIMMGrid& r_hg );
		// ---------------------- 内部操作 --------------------------------
		private:
			inline void generator();
			inline void inc_vec_gen( double x, double y );
			inline void dec_vec_gen( double x0, double y0, double x1, double y1 );
		// ---------------------- 内部参数及引用 --------------------------------
		private:
			// 外部类索引
			Position2DBridge* 	p_pos2d;
			RangerBridge*		p_ranger;
			uvectors_t*			p_uvectors;
			HIMMGrid*			p_himmgrid;
			// 内部参数
			Array2D<double>	_tpl;
			int 			_inc;
			int 			_dec;
			double			_max_reading;
	};
	HIMMUVGen& operator>>( RangerBridge& r_pos2d, HIMMUVGen& r_uvg );
}
	


#endif //_HIMMUVGEN_H_
