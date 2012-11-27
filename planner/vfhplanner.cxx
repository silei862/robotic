/*
 * =====================================================================================
 *
 *       Filename:  vfhplanner.cxx
 *
 *    Description:  使用Vector Field Histogram路径规划器实现
 *
 *        Version:  1.0
 *        Created:  2012年11月23日 16时59分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#include <debug.h>
#include <stdlib.h>
#include <misc.h>
#include <vector>
#include "vfhplanner.h"

typedef struct _BoundIndex
{
	size_t _begin;
	size_t _end;
} BoundIndex;

typedef std::vector<BoundIndex> BoundGroup;

using namespace SlamLab;

void SlamLab::gap_finder( PolarBinHist& r_pbh, DirectionGroup& r_dg ,Position2D pos )
{
	BoundGroup bounds;
	for( size_t i=0; i<r_pbh.sector_num() ; i++ )
	{
		int32_t begin=0;
		int32_t end=0;
		if( 0==r_pbh[i] )
		{
			begin=i;
			for( size_t j=i+1; j<r_pbh.sector_num(); j++ )
				if( 0==r_pbh[j] )
				{
					if( (r_pbh.sector_num()-1)==j )
					{
						end=r_pbh.sector_num()-1;
						i=j;
						break;
					}
					continue;
				}
				else
				{
					end=j-1;
					i=j;
					break;
				}
			if( end >= begin )
			{
				BoundIndex bound;
				bound._begin = begin;
				bound._end = end;
				bounds.push_back( bound );
			}
		}
	}
	if( bounds.empty() )
		return;
	// 处理首尾能够连接起来的情况
	if( 0 ==r_pbh[size_t(0)] && 0 ==r_pbh[r_pbh.sector_num()-1] )
	{
		// 计算头部尾部空方向扇区
		int32_t tail_width=bounds[bounds.size()-1]._end - bounds[bounds.size()-1]._begin+1;	
		int32_t head_width=bounds[0]._end - bounds[0]._begin+1;
		// 计算联接后方向扇区序号
		int32_t dir_index = ( head_width + tail_width )/2 - tail_width;
		if( dir_index < 0 )
			dir_index += int32_t(r_pbh.sector_num());
		// 生成节点：
		DirectionNode dir_node;
		dir_node._direction = double(dir_index)*r_pbh.sector_size()+r_pbh.sector_size()/2;
		dir_node._size = (tail_width + head_width)*r_pbh.sector_size();
		dir_node._pos =pos;
		r_dg.push_back( dir_node );
		bounds.pop_back();
	}
	else
	{
		DirectionNode dir_node;
		dir_node._direction = ( bounds[0]._end + bounds[0]._begin )*r_pbh.sector_size()/2;
		dir_node._size =( bounds[0]._end - bounds[0]._begin +1 )*r_pbh.sector_size();
		dir_node._pos = pos;
		r_dg.push_back( dir_node );
	}
	// 加入其他方向
	for( size_t i=1;i<bounds.size();i++ )
	{
		DirectionNode dir_node;
		dir_node._direction = ( bounds[i]._end + bounds[i]._begin )*r_pbh.sector_size()/2;
		dir_node._size = ( bounds[i]._end - bounds[i]._begin +1 )*r_pbh.sector_size();
		dir_node._pos = pos;
		r_dg.push_back( dir_node );
	}
}

std::ostream& std::operator<<( std::ostream& os , SlamLab::DirectionGroup& r_dg )
{
	os<<"Directions: number="<<r_dg.size()<<std::endl; 
	for( size_t i=0; i<r_dg.size(); i++ )
		os<<"["<<i+1<<"]:"<<"direction="<<R2D(r_dg[i]._direction)<<" width="<<R2D(r_dg[i]._size)<<" pos=["<<r_dg[i]._pos._x<<","<<r_dg[i]._pos._y<<"]"<<std::endl;
	return os;
}
