/*
 * =====================================================================================
 *
 *       Filename:  himmuvgen.h
 *
 *    Description:  HIMM跟新向量生成器
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
			HIMMUVGen( double cell_size,
					   int inc = DEF_INC,
					   int dec = DEF_DEC, 
					   double max_reading = def_max_reading );
			~HIMMUVGen(){ }
		public:
			inline HIMMUVGen& operator()( Position2DBridge& r_pos2d );
			inline uvectors_t& operator>>( uvectors_t& r_uv );
		// ---------------------- 内部操作 --------------------------------
		private:
			inline void set_pos2d( Position2DBridge& r_pos2d );
			inline void set_ranger( RangerBridge& r_ranger );
			inline void set_uvector( uvectors_t& r_uv );
			inline void generator();
			inline void inc_vec_gen( double x, double y );
			inline void dec_vec_gen( double x0, double y0, double x1, double y1 );
		// ---------------------- 内部参数及引用 --------------------------------
		private:
			// 传感器索引
			Position2DBridge* 	p_pos2d;
			RangerBridge*		p_ranger;
			uvectors_t*			p_uvectors;
			// 内部参数
			Array2D<double>	_tpl;
			int 			_inc;
			int 			_dec;
			double			_max_reading;
			double			_cell_size;
	};
}
	


#endif //_HIMMUVGEN_H_
