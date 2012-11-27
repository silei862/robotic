/*
 * =====================================================================================
 *
 *       Filename:  himmbuilder.h
 *
 *    Description:  Histogramic in-motion mapping的实现
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 21时50分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _HIMMBUILDER_H_
#define _HIMMBUILDER_H_

#include <array2d.h>
#include <bridge.h>
#include "himmgrid.h"

namespace SlamLab
{
	class HIMMBuilder
	{
		enum _CONST
		{
			DEF_TPLSIZE =3,
			TPL_NUM = DEF_TPLSIZE*DEF_TPLSIZE,
			DEF_INC	=5,
			DEF_DEC =1,
			DEF_MAX =15
		};

		private:
			static const double tpl_val[TPL_NUM];
			static const double max_reading;
			static const double min_reading;
		public:
			HIMMBuilder( uint8_t inc=DEF_INC,		//单次增量值
					uint8_t dec=DEF_DEC,			//消除干扰减量
					uint8_t max=DEF_MAX );			//网格允许最大值

			virtual ~HIMMBuilder(){ };

		public:
			inline void attach_position2d( Position2DBridge& r_pb ){ p_pos2d = &r_pb; }
			inline void attach_ranger( RangerBridge& r_rb ){ p_ranger = &r_rb; }
			inline void attach_map( HIMMGrid& hg ){ p_map = &hg; }
			void build();
			//有用的操作符
			HIMMGrid& operator>>( HIMMGrid& r_hg );
		private:
			void cell_inc(double x,double y);
			void cell_dec(double x0,double y0,double x1,double y1);

		private:
			// 外部器件：
			Position2DBridge* p_pos2d;
			RangerBridge* p_ranger;
			HIMMGrid* p_map;
			// 内部参数：
			Array2D< double > _tpl;
			uint8_t _inc;
			uint8_t _dec;
			uint8_t _max;
	};

	HIMMBuilder& operator>>( RangerBridge& r_rb , HIMMBuilder& r_builder );
}

#endif	//_HIMMBUILDER_H_
