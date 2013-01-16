/*
 * =====================================================================================
 *
 *       Filename:  dsgplanner.h
 *
 *    Description:  基于距离网格的全局导航类
 *
 *        Version:  1.0
 *        Created:  2012年12月29日 13时02分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _DSGPLANNER_H_
#define _DSGPLANNER_H_
#include <list>
#include <vector>
#include <coord.h>
#include <distancegrid.h>
//#define _EXPAND_ALL_NODE_

namespace SlamLab
{
	// 路径坐标：
	typedef std::vector< float_pos_t > path_t;
	// 基于距离网格地图的导航类：
	class DSGGuider
	{
		// ----------------- 内部使用类型 ------------------
		struct exnode_t
		{
			grid_pos_t	_pos;
			uint8_t		_direction;
			bool		_inopen;
			double		_heuristic;
			double		_depth;
			exnode_t*	p_parent;
		};
		typedef std::list< exnode_t > exnodelist_t;

		// ----------------- 默认参数 -----------------------
		static const size_t def_max_node_num = 64*1024;
		static const double def_h_depth = 10;
		static const double def_h_destination = 20;
		static const double def_h_dvalue = 80;
		// 状态迁移操作
		struct statetrans_t
		{
			int		_dx;
			int		_dy;
			uint8_t	_dir;
		};
		// 可行位置常量
		const static size_t DIR_NUM = 8;
		const static size_t OP_NUM = 3;
		const static statetrans_t strans[DIR_NUM][OP_NUM];
		// --------------------------------------------------
		public:
			DSGGuider( DistanceMap& r_dmap , double safe_distance );
			virtual ~DSGGuider(){ }

		// ----------------- 对外接口 -----------------------
		public:
			void set_start( double x , double y , double th );
			void set_destination( double x, double y );
			bool get_path( path_t& r_path );
			// 参数设置：
			void set_heuristic_para( double k_depth , double k_dest , double k_dval );
			void set_nodelist_len( size_t len );

		// -----------------内部函数 ------------------------
		private:
			bool _get_path( path_t& r_path );
			// 节点操作：
			exnode_t& _get_first_opennode( bool& allclosed );
			void _node_to_path( exnode_t& r_node , path_t& r_path );
			void _fill_heuristic( exnode_t& r_node );
			void _insert_node( exnode_t& r_node );
			// 方位转换：
			uint8_t _dir_trans( double rad );

		// --------------------------------------------------
		private:
			// 启发函数系数：
			double h_depth;
			double h_destination;
			double h_dval;
			// 目标坐标：
			grid_pos_t  _dest_cell;
			// 使用的距离网格地图：
			DistanceMap* p_dmap;
			// 安全距离：
			double	_safe_distance;
			// 节点列表：
			exnodelist_t _nodelist;
			// 最大节点数：
			size_t _max_node_num;
	};
}

#endif //_DSGPLANNER_H_
