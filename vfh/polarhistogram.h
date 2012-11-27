/*
 * =====================================================================================
 *
 *       Filename:  polarhistogram.h
 *
 *    Description:  方位直方图的实现
 *
 *        Version:  1.0
 *        Created:  2012年11月21日 11时29分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _POLARHISTOGRAM_H_
#define _POLARHISTOGRAM_H_

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <cmath>

namespace SlamLab
{
	template< typename TP >
	class PolarHistogram
	{
		public:
			static const size_t SECTOR_NUM=72;
			static const double PI=3.1415926535;
		public:
			PolarHistogram( size_t sector_num=SECTOR_NUM )
				:_sector_size( 2*PI/double(sector_num) ){ _histogram.resize( sector_num ); }
			virtual ~PolarHistogram() { }

		public:
			size_t sector_num( ){ return _histogram.size(); }
			double sector_size( ){ return _sector_size; }
			// 设置直方图内所有值
			void set_all_val( TP val )
			{
				for( size_t i=0;i<_histogram.size();i++ )
					_histogram[i] = val;
			}
			// 获取元素引用
			TP& get_ref( size_t index ){ return _histogram[index]; }
			TP& get_ref( double polar )
			{
				//double _polar=fmod( polar,2*PI );
				return _histogram[ size_t(polar/_sector_size) ];
			}
			// 判断整型索引是否越界
			bool in( size_t index ){ return index >=0 && index<_histogram.size(); }
			// 存取操作符
			TP& operator[]( size_t index ){ return get_ref( index ); }
			TP& operator[]( double polar ){ return get_ref( polar ); }
		private:
			std::vector<TP> _histogram;
			double _sector_size;
	};
}

#endif //_POLARHISTOGRAM_H_
