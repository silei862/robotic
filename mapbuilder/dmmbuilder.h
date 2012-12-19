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

#include <stdint.h>
#include <list>
#include <coord.h>
#include <himmgrid.h>
#include <distancegrid.h>
#include <updatevector.h>

namespace SlamLab
{

	class DMMBuilder
	{
		enum _CONST
		{
			DELTA_NUMS = 8,
		};
		// 周边八格坐标计算数组：
		const static int delta_i[DELTA_NUMS];
		const static int delta_j[DELTA_NUMS];
		// 增加更新描述单元
		struct add_unit_t
		{
			double		_update_val;
			size_t		_order_val;
			size_t		_level;
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
			DMMBuilder( int32_t max_val , int32_t th_low , int32_t th_high, double win_size );
			virtual ~DMMBuilder(){ }

		public:
			// 外部接口：
			void update();
			// 操作符部分：
			DMMBuilder& operator()( HIMMGrid& r_camp, double rx , double ry );
			DistanceMap& operator>>( DistanceMap& r_dmap );
			// 关联对象设置：
			void set_update_vector( uvectors_t& r_uvecs );
			void set_cmap( HIMMGrid& r_cmap );
			void set_dmap( DistanceMap& r_dmap );

		private:
			// 计算更新单元更新距离值：
			void _fill_unit_update_val( add_unit_t& r_au );
			// 障碍增加队列单元序值计算
			void  _fill_unit_order_val( add_unit_t& r_au );
			// 更新单元插入队列
			void _insert_addunit( add_unit_t& r_au );
			void _insert_clrunit( clr_unit_t& r_cu ); 
			// 障碍格增加：
			void _add_obstacle( double x , double y );
			// 障碍格清除：
			void _clear_obstacle( double x , double y );
			// 障碍增加更新计算：
			void _addobstacle_update();
			// 清除障碍更新计算：
			void _clearobstacle_update();
			// 通过序号获取网格单元周围八个格子：
			bool _to_around( grid_pos_t& r_pos , size_t idx );
		private:
			uvectors_t*		p_uvecs;
			HIMMGrid*		p_cmap;
			DistanceMap*	p_dmap;
			// 更新队列：
			std::list< add_unit_t >	_add_queue;
			std::list< clr_unit_t > _clr_queue;
			// 地图更新参数
			int32_t _max_val;
			int32_t _th_low;
			int32_t _th_high;
			// 活动窗口边长
			double _win_size;
			// 移动机器人中心坐标
			float_pos_t _robot_pos;
	};
	DMMBuilder& operator>>( uvectors_t& r_uvec , DMMBuilder& r_dmmbd );
}

#endif //_DMMBUILDER_H_
