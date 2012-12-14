/*
 * =====================================================================================
 *
 *       Filename:  updatevector.h
 *
 *    Description:  更新向量
 *
 *        Version:  1.0
 *        Created:  2012年12月14日 09时05分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _UPDATEVECTOR_H_
#define _UPDATEVECTOR_H_

#include <stdint.h>
#include <coord.h>
#include <vector>
namespace SlamLab
{
	typedef struct _update_vector_t
	{
		double 	x;
		double 	y;
		int32_t	delta;
	} update_vector_t;

	typedef struct _dm_update_cell_t
	{
		size_t				i;
		size_t				j;
		Point2D<double>	ob_pos;
		double				val;
	} dm_update_cell_t;

	typedef std::vector< update_vector_t > uvectors_t;
}

#endif //_UPDATEVECTOR_H_
