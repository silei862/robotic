/*
 * =====================================================================================
 *
 *       Filename:  planner_types.h
 *
 *    Description:  路径规划器使用的数据类型
 *
 *        Version:  1.0
 *        Created:  2012年11月23日 16时55分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _PLANNER_TYPES_H_
#define _PLANNER_TYPES_H_

#include <vector>
#include <types.h>

namespace SlamLab
{
	// 方位节点
	typedef struct
	{
		double 		_direction;
		double		_size;
		Position2D	_pos;

	} DirectionNode;
	
	// 方位节点组
	typedef std::vector< DirectionNode > DirectionGroup;

}

#endif //_PLANNER_TYPES_H_
