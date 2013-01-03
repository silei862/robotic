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
#include <coord.h>
#include <distancegrid.h>

namespace SlamLab
{
	typedef Point2D< double > position_t;
	typedef Point2D< size_t > int_pos_t;

	// 控制向量：
	struct control_t
	{
		double _v;	//线速度
		double _w;	//角速度
		size_t _tl;	//执行时长(毫秒)
	};
	typedef std::vector< control_t > ctrlgroup_t;

	// 状态向量
	struct state_t
	{
		double		_x;		//位置
		double 		_y;
		double		_th;	//姿态角
		control_t	_ctrl;	//对应控制向量
	};

	// 展开的节点
	struct exnode_t
	{
		state_t		_state;
		exnode_t* 	p_parent;
		double		_depth;
		double		_heuritic;
		bool		_in_open;
	};
	typedef std::list< exnode_t > nodelist_t;
	typedef std::vector< exnode_t > nodevector_t;


	// 导航类
	class DSGGuider
	{
		// ----------------- 默认参数 -----------------------
		// 启发函数系数：
		static const double def_k_dp = 10;
		static const double def_k_v = 10;
		static const double def_k_w = 10;
		static const double def_k_d2d = 80;
		// 默认线速度、角速度区间：
		static const double def_min_v = 0.1;
		static const double def_max_v = 0.8;
		static const double def_min_w = -0.7;
		static const double def_max_w = 0.7;
		// 默认线速度、角速度粒度：
		static const double def_delta_v = 0.1;
		static const double def_delta_w = 0.1;
		// 默认时间粒度：
		static const size_t def_delta_t = 100;
		// 默认安全距离：
		static const double def_safe_distance = 1.0;
		// 默认深度倍数：
		static const int mul_depth = 3;
		// 默认队列长度
		static const size_t def_max_listlen = 1024*512;
		
		public:
			DSGGuider( DistanceMap& rdmap );
			
		public:
			void set_start( double x , double y , double th );
			void set_destination( double x , double y, double th );
			bool get_controls( ctrlgroup_t& ctrls );
		// ----------------- 参数设置 -----------------------
			void set_weight( double wdp , double wv  , double ww , double wd2d );
			void set_speed_interval( double min_v , double max_v );
			void set_angular_speed_interval( double min_w , double max_w );
			void set_delta_speed( double dv , double dw );
			void set_time_delta( size_t dt );
			void set_safe_distance( double sd );
			void set_distance_map( DistanceMap& rdmap );
			void set_max_depth( double depth );
			void set_max_listlen( size_t maxlen );

		//------------------ 内部使用函数 -------------------
		private:
			// 获取_in_open=true的第一个结点
			exnode_t& _get_firstnode_in_open( bool& all_closed );
			// 获取所有孩子节点：
			void _childnode_gen( exnode_t& node , nodevector_t& chnodes );
			// 状态迁移计算及可行性检查：
			void _state_transfer( state_t& state );
			bool _state_applicable( state_t& state );
			// 到达目的检查：
			bool _dest_achived( exnode_t& node );
			// 控制向量生成器：
			void _ctrlvectors_gen( exnode_t& node , ctrlgroup_t& ctrls );
			// 计算距离：
			double _state_distance( state_t& st1 , state_t& st2 );
			double _pos_distance( state_t& st1 , state_t& st2 );
			// 节点插入表：
			void _insert_list( exnode_t& node );
			// 启发函数：
			void _fill_heuristic_val( exnode_t& node );
		private:
			// 启发函数各项系数：
			double k_dp;
			double k_v;
			double k_w;
			double k_d2d;
			// 节点允许最大深度：
			double _max_depth;
			// 最大节点列表长度：
			size_t _max_listlen;
			// 线速度、角速度参数：
			double _min_v;	// 最小速度
			double _max_v;	// 最大速度
			double _max_w;	// 角速度上限
			double _min_w;	// 角速度下限
			double _delta_v; // 线速度变化粒度
			double _delta_w; // 角速度变化粒度
			// 时间控制粒度
			size_t _delta_t; //执行机构控制周期(毫秒)
			// 安全距离值：
			double _safe_distance;
			// 使用的距离网格地图：
			DistanceMap*	p_dmap;
			// 目标状态和当前状态：
			state_t	_dest_state;
			state_t	_start_state;
			double _tolarance;	
			// 存储展开节点的表
			nodelist_t	_node_list;
	};
}

#endif //_DSGPLANNER_H_
