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
	// 速度向量
	struct speed_t
	{
		double _v;	//线速度
		double _w;	//角速度
	};
	// 状态向量
	struct state_t
	{
		position_t	_pos;	//位置
		double		_th;	//姿态角
		speed_t		_speed;	//速度向量
	};

	// 展开的节点
	struct exnode_t
	{
		state_t	_state;
		exnode_t* p_parent;
		double	_depth;
		double	_heuritic;
	};
	typedef std::list< exnode_t > nodelist_t;
	typedef std::vector< exnode_t > nodevector_t;

	// 控制向量：
	struct control_t
	{
		speed_t		_ctl;		// 控制向量
		position_t	_dest_pos;	// 目标位置
		double		_dest_th;	// 目标姿态
	};
	typedef std::vector< control_t > ctrlgroup_t;

	// 导航类
	class DSGGuider
	{
		public:
			DSGGuider();
			
		public:
			void init( state_t& cur , state_t& dest );
			void set_dest( state_t& dest );
			bool get_controls( ctrlgroup_t& ctrls );
		//------------------ 内部使用函数 -------------------
		private:
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
			double _pos_distance( position_t& pos1 , position_t& pos2 );
			// 节点插入表：
			void _insert_openlist( exnode_t& node );
			void _insert_closelist( exnode_t& node );
			void _insert_list( nodelist_t& list , exnode_t& node );
			// 启发函数：
			void _fill_heuristic_val( exnode_t& node );
		private:
			// 启发函数各项系数：
			double k_dp;
			double k_v;
			double k_w;
			double k_d2d;
			// 线速度、角速度参数：
			int _v_mul;	// 线速度最大倍数
			int _w_mul;	// 角速度最大倍数
			int _v_delta_mul; // 线速度变化最大倍数
			int _w_delta_mul; // 叫速度变化最大倍数
			double _delta_v; // 线速度变化粒度
			double _delta_w; // 角速度变化粒度
			// 状态迁移步长：
			double _step;
			// 安全距离值：
			double _safe_distance;
			// 使用的距离网格地图：
			DistanceMap*	p_dmap;
			// 目标状态和当前状态：
			state_t	_dest_state;
			state_t	_cur_state;
			double _tolarance;	
			// 存储展开节点的表
			nodelist_t	_open_list;
			nodelist_t	_close_list;
	};
}

#endif //_DSGPLANNER_H_
