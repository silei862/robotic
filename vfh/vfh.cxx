/*
 * =====================================================================================
 *
 *       Filename:  vfh.cxx
 *
 *    Description:  VFH实现文件
 *
 *        Version:  1.0
 *        Created:  2012年11月21日 13时33分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <vector>
#include <cmath>
#include <grid.h>
#include <exception.h>
#include <misc.h>
#include <debug.h>
#include "vfh.h"
#include "mapwin.h"

using namespace SlamLab;

// ---------------------- PHBuilder -----------------------------------------------------
void PHBuilder::build()
{
	ASSERT(p_map&&p_histo);
	// 计算参数：
	double ws = (2*_radius-1)/2;
	double A = 1 + B*ws*ws;
		
	MapWin himm_win( _center_pos._x, _center_pos._y, _radius , p_map, bound_val );
	p_histo->set_all_val(0);

	for( size_t y=0; y<himm_win.height() ; y++ )
		for( size_t x=0; x<himm_win.width(); x++ )
		{
			
			// 获取单元格坐标并计算距离
			Point2D<double> cell_pos=himm_win.cell_coord( x , y );
			double delta_x = cell_pos._x - _center_pos._x;
			double delta_y = cell_pos._y - _center_pos._y;
			double distance = sqrt( delta_x*delta_x + delta_y*delta_y );
			// 计算方位角度
			double bearing = acos( delta_x/distance );
			if( delta_y < 0 )
				bearing =2*PI-bearing;
			// 根据角度获取扇区
			double polar_val =double((*p_histo)[bearing]);
			// 获取网格值
			double cell_val = double(himm_win( x, y ));
			// 计算累加值
			polar_val+=cell_val*cell_val*(A - B*distance);
			// 保存计算结果
			(*p_histo)[bearing] = int32_t(polar_val);
		}
	//smooth_hist();
}

/* void PHBuilder::smooth_hist( )
{
	std::vector<int32_t> phsm;
	phsm.resize(p_histo->sector_num());
	for( size_t i=L; i<p_histo->sector_num()-L; i++ )
	{
		uint64_t sum=L*(*p_histo)[i];
		for(size_t k=0;k<L-1;k++ )
			sum+=(k+1)*(*p_histo)[i-L+k]+(k+1)*(*p_histo)[i+L-k];
		phsm[i]=sum/(2*L+1);
	}
	for( size_t i=L; i<phsm.size()-L;i++ )
		(*p_histo)[i] = phsm[i];
}*/

PHBuilder& PHBuilder::operator()( double x ,double y )
{
	set_center( x, y);
	return *this;
}

PHBuilder& PHBuilder::operator()( double x ,double y, double rd )
{
	set_center( x,y );
	set_radius( rd );
	return *this;
}

PolarHist& PHBuilder::operator>>( PolarHist& r_ph )
{
	set_histogram( &r_ph );
	build();
	return r_ph;
}

PHBuilder& SlamLab::operator>>( HIMMGrid& r_map,PHBuilder& r_phb )
{
	r_phb.set_map( &r_map );
	return r_phb;
}

// ------------------------- BinPHBuilder --------------------------------
void BinPHBuilder::build()
{
	ASSERT( p_polarhist && p_binhist );
	//ASSERT( p_polarhist->size() == p_binhist->size() );
	init_threshold();
	for( size_t i=0; i<p_polarhist->sector_num(); i++ )
	{
		if( (*p_polarhist)[i] < _threshold_low )
			(*p_binhist)[i] = 0;
		if( (*p_polarhist)[i] > _threshold_high )
			(*p_binhist)[i] = 100;
	}
}

void BinPHBuilder::init_threshold()
{
	double sum=0;
	for( size_t i=0; i<p_polarhist->sector_num(); i++ )
		sum+=double((*p_polarhist)[i]);
	double average=sum/p_polarhist->sector_num();
	_threshold_high = int32_t( average ) - int32_t(average)/_resolution;
	_threshold_low = int32_t( average)/(_resolution*_resolution);
	INFO_VAR( _threshold_high );
	INFO_VAR( _threshold_low );
}

PolarBinHist& BinPHBuilder::operator>>( PolarBinHist& r_pbh )
{
	set_binhist( &r_pbh );
	build();
	return r_pbh;
}

BinPHBuilder& BinPHBuilder::operator()( int32_t resol )
{
	_resolution = resol;
	return (*this);
}

BinPHBuilder& SlamLab::operator>>( PolarHist& r_ph , BinPHBuilder& r_bpb )
{
	r_bpb.set_polarhist( &r_ph );
	return r_bpb;
}

// ------------------- Output Stream For PolarHist -----------------------
std::ostream& std::operator<<( std::ostream& os , PolarHist& r_ph )
{
	for( size_t i=0 ; i<r_ph.sector_num(); i++ )
	{
		int32_t loop = r_ph[ i ]/80;
		os<<"["<<R2D(i*r_ph.sector_size())<<"]"<<"("<<r_ph[i]<<")";
		for( int32_t j=0; j<loop; j++ )
			os<<"=";
		os<<std::endl;
	}
	return os;
}

std::ostream& std::operator<<( std::ostream& os , PolarBinHist& r_pbh )
{
	for( size_t i=0; i<r_pbh.sector_num(); i++ )
	{
		os<<"["<<R2D(i*r_pbh.sector_size())<<"]";
		if( r_pbh[i] )
			os<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;
		else
			os<<std::endl;
	}
	return os;
}
