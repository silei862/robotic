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
#include <vector>
#include <distancegrid.h>

namespace SlamLab
{
	// ------------ 默认参数 --------------
	static const double def_safe_distance = 0.5;
	// 路径类型：
	typedef std::vector< float_pos_t > path_t;

	class HybirdAstar
	{
	// ------------ 常量 --------------------
		static const double def_h_depth = 10;
		static const double def_h_theta = 10;
		static const double def_h_todest = 10;
		// 运动限制参数：
		static const double def_delta_v = 0.1;
		static const double def_max_delta_v = 0.3;
		static const double def_delta_w = 0.1;
		static const double def_max_delta_w = 0.3;
		static const double def_min_v = 0.1;
		static const double def_max_v = 1.0;
		static const double def_min_w = -0.3;
		static const double def_max_w = 0.3;

	// ------------ 类型 -----------------
		// 混合状态：
		struct mixstate_t
		{
			grid_pos_t	_gpos;
			double		_x;
			double		_y;
			double		_th;
			double		_v;
			double		_w;
		};

		// 展开节点：
		struct hanode_t
		{
			mixstate_t	_state;
			double		_heuristic;
			double		_depth;
			hanode_t*	p_parent;
		};
		typedef std::vector< hanode_t* > nodevector_t;
		// 节点表：
		typedef std::list<hanode_t*> nodelist_t;

		public:
			static const size_t max_listlen = 1024*512;

		public:
			HybirdAstar( DistanceMap& r_dmap , double safe_d = def_safe_distance );
			~HybirdAstar();
		// -----------------  用户接口 ---------------
			// 设置起始状态、终点： 
			void set_start( double x , double y , double th , double v=0 , double w=0 );
			void set_destination( double x , double y );
			// 参数设置：
			void set_heuristic_para( double k_depth , double k_theta , double k_todest );
			void set_hspeed_delta( double delta_v , double max_delta_v );
			void set_hspeed_interval( double min_v , double max_v );
			void set_aspeed_delta( double delta_w , double max_delta_w );
			void set_aspeed_interval( double min_w , double max_w );
			void set_safe_distance( double safe_distance );
			// 获取路径:
			bool get_path( path_t& r_path );

		// ----------------- 内部函数 -----------------
		private:
			// 节点管理：
			hanode_t* create_node();
			hanode_t* create_node( mixstate_t& r_state, hanode_t* p_parent );
			void destroy_node( hanode_t* p_node );
			// --------------- 节点展开 -------------------
			void _node_to_path( hanode_t* p_node , path_t& r_path );
			void _expand_node( hanode_t* p_node , nodevector_t& r_nodes );
			void _add_node( nodevector_t& r_nodes , hanode_t* p_node );
			void _transfer_state( mixstate_t& r_state , double v , double w);
			bool _in_neighbor( grid_pos_t& r_centerpos , grid_pos_t& r_pos );
			// --------------- 计算启发函数 -----------------------
			void _fill_heuristic( hanode_t* p_node );
			double _state_distance( mixstate_t& r_st1, mixstate_t& r_st2 );
			double _pos_distance( float_pos_t& r_pos1 , float_pos_t& r_pos2 );
			double _to_destangular( mixstate_t& r_state );
			double _to_destdistance( mixstate_t& r_state );
			// 节点列表操作：
			void _insert_openlist( hanode_t* p_node );
			void _insert_closelist( hanode_t* p_node );
			void _insert_list( nodelist_t& r_list , hanode_t* p_node );
			// 目标到达检查：
			bool _destination_reached( grid_pos_t& r_gpos );
			
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
			// 启发函数系数：
			double h_depth;
			double h_theta;
			double h_todest;
			// 运动限制参数：
			double _delta_v;
			double _max_delta_v;
			double _delta_w;
			double _max_delta_w;
			double _min_v;
			double _max_v;
			double _min_w;
			double _max_w;
			// 状态迁移步长：
			double _step_len;
	};
}

#endif //_HYBIRDASTAR_H_
