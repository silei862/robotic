/*
 * =====================================================================================
 *
 *       Filename:  dmmbuilder.cxx
 *
 *    Description:  双地图更新类
 *
 *        Version:  1.0
 *        Created:  2012年12月17日 19时21分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <cmath>
#include <debug.h>
#include <dmmbuilder.h>

using namespace SlamLab;

// ------------------------------- 更新器初始化 --------------------------------
DMMBuilder::DMMBuilder()
	:p_uvec( NULL ) , p_cmap( NULL ) , p_dmap( NULL )
{

}

DMMBuilder::DMMBuilder( uvectors_t& r_uvec , HIMMGrid& r_cmap , DistanceMap& r_dmap )
	:p_uvec( &r_uvec ) , p_cmap( &r_cmap ) , p_dmap( &r_dmap )
{

}

// ------------------------------ 内部操作部分 ---------------------------------
void DMMBuilder::_unit_update_val( add_unit_t& r_au )
{
	ASSERT( p_cmap!=NULL  );
	float_pos_t pos = p_cmap->cell_coord( r_au._pos._x , r_au._pos._y );
	float_pos_t ob_pos = p_cmap->cell_coord( r_au._obstacle_pos._x , r_au._obstacle_pos._y );
		
}
