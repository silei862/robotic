/*
 * =====================================================================================
 *
 *       Filename:  mapclient.cxx
 *
 *    Description:  地图数据接收客户端实现
 *
 *        Version:  1.0
 *        Created:  2012年12月25日 11时31分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mapconvi.h>
#include <mapclient.h>

using namespace std;
using namespace SlamLab;

// --------------------- 对外接口 ---------------------------------
MapClient::MapClient( const string& addr , uint16_t port )
	:_ip_addr( addr ) , _port( port )
{

}

MapClient& MapClient::operator()( const string& addr , uint16_t port )
{
	_ip_addr = addr;
	_port = port;
	return *this;
}

HIMMGrid& MapClient::operator>>( HIMMGrid& r_hg )
{
	_setup_link();
	// 发出获取概率地图数据请求：
	request_t req;
	req.cmd_id = ID_GETMAP;
	req.map_type = ID_CVGRID_MAP;
	char* p_reqdata = reinterpret_cast<char*>( &req );
	_send_data( p_reqdata , sizeof( request_t ) );
	// 分配空间接受数据：
	char* p_data = new char[ RBUF_SIZE*2 ];
	size_t num = RBUF_SIZE*2;
	_recv_data( p_data , num );
	// 检测返回数据
	if( ID_CVGRID_MAP != p_data[0] )
	{
		_sv_retstr.copy( &p_data[1] , num -1 );
		delete[] p_data;
		throw( data_err() );
	}
	// 进行数据转换：
	Char2HIMMGrid char2hg;
	char2hg.set_data( p_data , r_hg );
	// 清理工作
	delete[] p_data;
	close( _fd );
	return r_hg;
}

DistanceMap& MapClient::operator>>( DistanceMap& r_dm )
{
	_setup_link();
	// 发出获取距离网格地图数据请求：
	request_t req;
	req.cmd_id = ID_GETMAP;
	req.map_type = ID_DSGRID_MAP;
	char* p_reqdata = reinterpret_cast<char*>( &req );
	_send_data( p_reqdata , sizeof( request_t ) );
	// 分配空间接受数据:
	char* p_data = new char[ RBUF_SIZE*2 ];
	size_t num = RBUF_SIZE*2;
	_recv_data( p_data , num );
	// 检测返回数据
	if( ID_DSGRID_MAP != p_data[0] )
	{
		_sv_retstr.copy( &p_data[1] , num -1 );
		delete[] p_data;
		throw( data_err() );
	}
	// 进行数据转换：
	Char2DistanceGrid char2dm;
	char2dm.set_data( p_data , r_dm );
	// 删除临时数据：
	delete[] p_data;
	close( _fd );
	return r_dm;
}

const string& MapClient::server_ret()
{
	return _sv_retstr;
}
// --------------------- 内部操作 --------------------------------

void MapClient::_setup_link()
{
	// 尝试建立套接字：
	_fd = socket( AF_INET , SOCK_STREAM , 0 );
	if( _fd < 0 )
		throw( setuplink_err() );
	_sv_addr.sin_family = AF_INET;
	_sv_addr.sin_port = htons( _port );
	_sv_addr.sin_addr.s_addr = inet_addr( _ip_addr.c_str() );
	// 尝试连接主机：
	int rc = connect( _fd , (sockaddr*)&_sv_addr , sizeof( _sv_addr ) );	
	if( rc != 0 )
		throw( setuplink_err() );
}

void MapClient::_send_data( char* p_data , size_t num )
{
	size_t pos = 0;
	for( ; ; )
	{
		size_t num_sd = send( _fd , &p_data[pos] , num-pos , 0 );
		pos += num_sd;
		if( pos >= num )
			break;
	}
}

void MapClient::_recv_data( char*& p_data , size_t& num )
{
	char* p_rbuf = new char[ RBUF_SIZE ];
	if( !p_rbuf )
		throw( alloc_fail() );
	size_t pos = 0;
	// 接受所有数据：
	for( ; ; )
	{
		size_t num_rd = recv( _fd , p_rbuf , RBUF_SIZE , 0 );
		if( 0 == num_rd )
			break;
		if( ( pos + num_rd )> num )
		{
			// 扩展目标空间，并保存原有数据
			char* p_tmp = new char[ num + RBUF_SIZE ];
			if( !p_tmp )
			{
				delete[] p_rbuf;
				throw( alloc_fail() );
			}
			memcpy( p_tmp , p_data , num );
			num += RBUF_SIZE;
			delete[] p_data;
			p_data = p_tmp;
		}
		// 数据填充：
		memcpy( &p_data[pos] , p_rbuf , num_rd );
		pos += num_rd;
	}
	delete[] p_rbuf;
}
