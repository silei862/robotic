/*
 * =====================================================================================
 *
 *       Filename:  hybirdastar.h
 *
 *    Description:  混合A＊路径搜索算法
 *
 *        Version:  1.0
 *        Created:  2013年01月03日 09时04分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _HYBIRDASTAR_H_
#define _HYBIRDASTAR_H_

#include <list>
#include <distancegrid.h>

namespace SlamLab
{
	// ------------ 默认参数 --------------
	static const double def_safe_distance = 0.5;
	// 路径类型：
	typedef std::vector< float_pos_t > path_t;

	class HybirdAstar
	{
	// ------------ 类型 -----------------
		// 混合状态：
		struct mixstate_t
		{
			grid_pos_t	_gpos;
			double		_x;
			double		_y;
			double		_th;
		};

		// 展开节点：
		struct hanode_t
		{
			mixstate_t	_state;
			double		_v;
			double		_w;
			double		_heuristic;
			double		_depth;
			hanode_t*	p_parent;
		};
		// 节点表：
		typedef std::list<hanode_t*> nodelist_t;

		public:
			static const size_t max_listlen = 1024*512;

		public:
			HybirdAstar( DistanceMap& r_dmap , double safe_d = def_safe_distance );
			~HybirdAstar();
		// -----------------  用户接口 ---------------
			// 设置起始状态、终点： 
			void set_start( double x , double y , double th, double v = 0  , double w = 0 );
			void set_destination( double x , double y );
			// 获取路径:
			bool get_path( path_t& r_path );

		// ----------------- 内部函数 -----------------
		private:
			// 节点管理：
			hanode_t* create_node();
			hanode_t* create_node( mixstate_t& r_state, double v = 0 , double w = 0  );
			void destroy_node( hanode_t* p_node );
			// 计算启发函数：
			void _fill_heuristic( hanode_t* p_node );
			double _state_distance( mixstate_t& r_st1, mixstate_t& r_st2 );
			double _pos_distance( float_pos_t& r_pos1 , float_pos_t& r_pos2 ); 
			// 节点列表操作：
			void _insert_openlist( hanode_t* p_node );
			void _insert_closelist( hanode_t* p_node );
			void _insert_list( nodelist_t& r_list , hanode_t* p_node );
			
		private:
			// 安全距离：
			double	_safe_distance;
			// 进行搜索的地图
			DistanceMap*	p_dmap;
			// 目的网格坐标：
			grid_pos_t		_dest_cell;
			// 节点列表
			nodelist_t		_openlist;
			nodelist_t		_closelist;
	};
}

#endif //_HYBIRDASTAR_H_
