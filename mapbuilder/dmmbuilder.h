/*
 * =====================================================================================
 *
 *       Filename:  dmmbuilder.h
 *
 *    Description:  双地图更新构建器
 *
 *        Version:  1.0
 *        Created:  2012年12月17日 11时31分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _DMMBUILDER_H_
#define _DMMBUILDER_H_

#include <coord.h>
#include <himmgrid.h>
#include <distancegrid.h>
#include <updatevector.h>

namespace SlamLab
{

	class DMMBuilder
	{
		// 内部使用类型：
		typedef Point2D<size_t> grid_pos_t;
		typedef Point2D<double> float_pos_t;

		// 增加更新描述单元
		struct add_unit_t
		{
			double		_update_val; 		
			grid_pos_t	_pos;
			grid_pos_t	_obstacle_pos;
		};
		// 消除更新描述单元
		struct clr_unit_t
		{
			grid_pos_t _pos;
			grid_pos_t _obstacle_pos;
		};

		public:
			DMMBuilder();
			DMMBuilder( uvectors_t& r_uvec , HIMMGrid& r_cmap , DistanceMap& r_dmap );
			virtual ~DMMBuilder(){ }
		public:
			DMMBuilder& operator()( HIMMGrid& r_camp , DistanceMap& r_dmap );
			HIMMGrid& operator>>( HIMMGrid& r_cmap );
			DistanceMap& operator>>( DistanceMap& r_dmap );
		private:
			// 关联对象设置：
			inline void _set_update_vector( uvectors_t& r_uvec );
			inline void _set_cmap( HIMMGrid& r_cmap );
			inline void _set_dmap( DistanceMap& r_dmap );
			// 计算更新单元更新距离值：
			void _unit_update_val( add_unit_t& r_au );


		private:
			uvectors_t*		p_uvec;
			HIMMGrid*		p_cmap;
			DistanceMap*	p_dmap;
			
	};

}

#endif //_DMMBUILDER_H_
