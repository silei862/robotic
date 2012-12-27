/*
 * =====================================================================================
 *
 *       Filename:  mapconvi.h
 *
 *    Description:  地图数据转换类模板
 *
 *        Version:  1.0
 *        Created:  2012年12月24日 15时11分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _MAPCONVI_H_
#define _MAPCONVI_H_

#include <stdlib.h>
#include <stdint.h>
#include <himmgrid.h>
#include <distancegrid.h>

namespace SlamLab
{
	// 传送数据，保存地图数据所用结构
	struct grid_head_t
	{
		uint8_t	_id;
		size_t	_cols;
		size_t	_rows;
		double	_cell_size;
		double	_origin_x;
		double	_origin_y;
	};
// ----------------------------------- 地图转字节 --------------------------------------	
	template< typename MTP , typename DTP >
		class Map2Char
		{
			public:
				class alloc_fail{ };
			public:
				Map2Char( )
					:_data( NULL ),_size(0){ }
				virtual ~Map2Char()
				{
					if( _data )
						delete[] _data;
				}

			public:
				inline void set_map( MTP& r_gm , uint8_t id )
				{ 
					// 填充地图头部数据:
					grid_head_t grid_head;
					grid_head._id = id;
					grid_head._cols = r_gm.cols();
					grid_head._rows = r_gm.cols();
					grid_head._cell_size = r_gm.cell_size();
					grid_head._origin_x = r_gm.origin()._x;
					grid_head._origin_y = r_gm.origin()._y;

					// 计算头部、数据部分、数据总大小：
					size_t head_size = sizeof( grid_head_t );	
					size_t map_size = grid_head._cols*grid_head._rows;
					size_t map_bytes = sizeof( DTP )*map_size;
					size_t size = head_size + map_bytes;
					// 如果数据空间不符合
					if( _size!=size )
					{
						// 删除旧数据
						if( _data )
							delete[] _data;
						// 创建新数据
						_data = new char[ size ];
						if( !_data )
						{
							_size = 0 ;
							throw( alloc_fail() );
						}
						_size = size;
					}
					// 写入头部数据：
					char* p_head = reinterpret_cast< char* >( &grid_head );
					for( size_t i = 0 ; i < head_size ; i++ )
						_data[i] = p_head[ i ];

					// 抓取地图数据：
					DTP* p_map_data = new DTP[ map_size ];
					if( !p_map_data )
					{
						delete[] _data;
						throw( alloc_fail() );
					}
					for( size_t j = 0 ; j < grid_head._rows ; j++ )
						for( size_t i = 0 ; i < grid_head._cols ; i++ )
							p_map_data[ j*grid_head._cols + i ] = r_gm( i , j );

					// 写入data
					char* p_map_char = reinterpret_cast< char* >( p_map_data );
					for( size_t i = 0 ; i < map_bytes ; i++ )
						_data[ head_size + i ] = p_map_char[ i ];
					// 删除临时地图数据
					delete p_map_data; 
				}

				inline size_t size(){ return _size; }
				inline char* data(){ return _data; }
			private:
				size_t	_size;
				char*	_data;
		};
	// 定义实际地图转换器：
	typedef Map2Char< HIMMGrid , uint8_t > HIMMGrid2Char;
	typedef Map2Char< DistanceMap , dm_cell_t > DistanceGrid2Char;

// ----------------------------------- 字节数据转地图 -----------------------------------------
	template< typename MTP , typename DTP >
		class Char2Map
		{
			// 异常类
			public:
				class alloc_fail{ };
			public:
				Char2Map()
					:p_map( NULL ){ }
				virtual ~Char2Map(){ }
			
			public:
				void set_data( char* p_data, MTP& r_map )
				{
					// 数据检查：
					if( !p_data )
						return;
					// 填充头部数据：
					grid_head_t* p_head = new grid_head_t;
					if( !p_head )
						throw( alloc_fail() );
					char* p_head_bytes = reinterpret_cast< char* >( p_head );
					for( size_t i = 0 ; i < sizeof( grid_head_t ) ; i++ )
						p_head_bytes[i] = p_data[ i ];

					// 获取各类数据大小：
					size_t map_size = p_head->_cols * p_head->_rows;
					size_t map_bytes = map_size*sizeof( DTP );
					size_t head_size = sizeof( grid_head_t );
					// 为地图内容数据分配空间，并作合法性检查：
					DTP* p_map_data = new DTP[ map_size ];
					if( !p_map_data )
					{
						delete p_head;
						throw( alloc_fail() );
					}
					// 填充地图内容数据：
					char* p_map_bytes = reinterpret_cast< char* >( p_map_data );
					for( size_t i = 0 ; i < map_bytes ; i++ )
						p_map_bytes[ i ] = p_data[ head_size + i ];
					// 设置地图指针：
					p_map = &r_map;
					// 重设地图属性：
					p_map->set_attr( p_head->_cols , p_head->_rows , p_head->_cell_size,
							Point2D<double>( p_head->_origin_x , p_head->_origin_y ));
					// 将数据转移到地图中：
					for( size_t j = 0 ; j < p_map->rows(); j++ )
						for( size_t i = 0 ; i < p_map->cols(); i++ )
							(*p_map)( i , j ) = p_map_data[ j*p_head->_cols + i ];
					// 保存地图id
					_id = p_head->_id;
					// 删除临时数据：
					delete p_map_data;
					delete p_head;
				}
				// 获取地图id
				uint8_t map_id(){ return _id; };
				// 获取地图引用：
				MTP& map(){ return *p_map; }
			private:
				uint8_t		_id;
				MTP*		p_map;
		};
	// 定义实际转换器：
	typedef Char2Map< HIMMGrid , uint8_t > Char2HIMMGrid;
	typedef Char2Map< DistanceMap , dm_cell_t > Char2DistanceGrid;
}

#endif //_MAPCONVI_H_
