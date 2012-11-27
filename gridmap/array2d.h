/*
 * =====================================================================================
 *
 *       Filename:  array2d.h
 *
 *    Description:  二维数组
 *
 *        Version:  1.0
 *        Created:  2012年10月15日 16时44分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */

#ifndef _ARRAY2D_H_
#define _ARRAY2D_H_

#include <stdint.h>
#include <vector>
#include <iostream>

#include "debug.h"

namespace SlamLab
{
	template< typename TP >
	class Array2D;

	// 	Class Name: Array2Dindexer
	// Description: 提供[][]存取能力的辅助类
	template< typename TP >
	class Array2Dindexer
	{
		friend class Array2D<TP>;
	
		public:
			Array2Dindexer( Array2D<TP>* p_arr )
				:p_arr(p_arr){ }

		public:
			inline TP& operator[]( uint32_t x ){ return p_arr->ref_at(x,y_index); }

		private:
			uint32_t y_index;
			Array2D<TP>* p_arr;
	};

//template < typename TP > ostream& operator<<(ostream &out,Array2D<TP> &arr);

// 	Class Name：Array2D
// Description: 提供二维坐标存取的能力
template< typename TP >
class Array2D
{
//	friend ostream& operator<< <TP>(ostream &out, Array2D<TP> &arr);
	public:
		Array2D()
			:_indexer(this){ }

		Array2D( uint32_t col, uint32_t row )
			:_col_num(col),_row_num(row),_indexer(this)
		{
			_data.resize(_col_num*_row_num);
		}
		
		// 一维数据二维化
		Array2D( TP* p_data, uint32_t col, uint32_t row )
			:_col_num(col),_row_num(row),_indexer(this)
		{
			_data.resize(_col_num*_row_num);
			for(uint32_t i=0;i<_col_num*_row_num;i++)
				_data[i] = p_data[i];
		}
		
		Array2D( const Array2D<TP>& arr )
			:_col_num(arr._col_num),_row_num(arr._row_num),_indexer(this)
		{
			_data.resize(_col_num*_row_num);
			for(uint32_t i=0;i<_col_num*_row_num;i++)
				_data[i] = arr._data[i];
		}

		virtual ~Array2D(){}
	
	public:
		// 基本属性:
		inline uint32_t row_size(){ return _row_num; }
		inline uint32_t col_size(){ return _col_num; }
		
		// 存取能力:
		inline TP& ref_at( uint32_t x, uint32_t y )
		{
			ASSERT( (x<_col_num)&&(y<_row_num) );
			return _data[y*_col_num+x];
		}

		inline void set_value( uint32_t x, uint32_t y, const TP& val )
		{
			ASSERT( (x<_col_num)&&(y<_row_num) );
			_data[y*_col_num+x] = val;
		}
		
		// 设置所有元素为val
		void set_all_val( const TP& val )
		{
			for( uint32_t i=0;i<_col_num*_row_num;i++ )
				_data[i] = val;
		}
		
		// 获取数据引用
		std::vector<TP>& data(){ return _data; }
	
		// 存储调整能力，当新尺寸小于原尺寸时，超出数据被丢弃
		void resize( uint32_t col, uint32_t row )
		{
			uint32_t min_col,min_row;
			// 首先保存原有数据：
			Array2D<TP> tmp(*this);
			// 从新尺寸和旧尺寸中挑选较小者
			min_col = ( _col_num<col )?_col_num:col;
			min_row = ( _row_num<row )?_row_num:row;
			// 设置新的属性并调整存储：
			_col_num = col;
			_row_num = row;
			_data.resize( _col_num*_row_num );
			
			//重置所有数据为0（测试使用）
			//set_all_to( 0 ); 
			
			//进行数据复制
			for(uint32_t y=0;y<min_row;y++)
				for(uint32_t x=0;x<min_col;x++)
					this->ref_at(x,y) = tmp.ref_at(x,y);
		}
		
		// 获取子二维数组：
		// （从x,y处开始，获取width宽，height高的子数组 )
		// 当指定的超出部分将被忽略，且子数组大小将根据实际改变
		Array2D<TP> sub_array( uint32_t x, uint32_t y, uint32_t width ,uint32_t height )
		{
			uint32_t end_x = ((x+width) < _col_num )?(x+width):_col_num;
			uint32_t end_y = ((y+height) < _row_num )?(y+height):_row_num;
			Array2D<TP> sub_arr(end_x-x,end_y-y);
			//数据复制
			for( uint32_t iy = y;iy<end_y;iy++ )
				for( uint32_t ix = x;ix<end_x;ix++ )
					sub_arr.ref_at( ix-x,iy-y ) = this->ref_at( ix,iy );
			
			return sub_arr; 
		}

		// 部分赋值：
		// 将数组sub_arr复制到x,y处，替换原有数据
		// 超出部分将被忽略
		void sub_assign( uint32_t x,uint32_t y, Array2D<TP>& sub_arr )
		{
			uint32_t end_x = ((x+sub_arr._col_num)<_col_num)?(x+sub_arr._col_num):_col_num;
			uint32_t end_y = ((y+sub_arr._row_num)<_row_num)?(y+sub_arr._row_num):_row_num;
			//数据复制：
			for( uint32_t iy = y;iy<end_y;iy++ )
				for( uint32_t ix = x;ix<end_x;ix++ )
					this->ref_at(ix,iy) = sub_arr.ref_at( ix-x,iy-y );
		}

		// 操作符：
			inline TP& operator()(uint32_t x, uint32_t y){ return ref_at( x, y ); }
			inline Array2Dindexer<TP>& operator[]( uint32_t y )
			{
				_indexer.y_index = y;
				return _indexer;
			}

			void operator= ( const Array2D<TP>& arr )
			{
				_col_num = arr._col_num;
				_row_num = arr._row_num;
				_data.resize( _col_num*_row_num );
				for( uint32_t i=0;i<_col_num*_row_num;i++ )
					_data[i] = arr._data[i];
			}
		protected:
			Array2Dindexer<TP> _indexer;	//提供[][]访问能力
		private:
			std::vector<TP> _data;
			uint32_t _col_num;
			uint32_t _row_num;
	};
}

namespace std
{
	// 数组元素输出，方便调试
	template< typename TP>
	ostream& operator<<(ostream &out , SlamLab::Array2D<TP> &arr)
	{
		out<<"[ Array Height:"<<arr.row_size()<<" Width: "<<arr.col_size()<<" ]"<<endl;
		for( uint32_t y=0;y<arr.row_size();y++)
		{
			for( uint32_t x=0;x<arr.col_size();x++)
				out<<arr[y][x]<<" ";
			out<<endl;
		}
		return out;
	}
}


#endif	//_ARRAY2D_H_
