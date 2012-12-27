/*
 * =====================================================================================
 *
 *       Filename:  mapserver.cxx
 *
 *    Description:  地图数据服务器实现
 *
 *        Version:  1.0
 *        Created:  2012年12月25日 11时13分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <mapconvi.h>
#include <mapserver.h>
using namespace std;
using namespace SlamLab;

MapServer::MapServer( uint16_t port )
	:_port( port )
{
	_exit_flag = false;
}

// ------------ 服务器控制函数 -------------------------------
void MapServer::init()
{
	_sv_fd = socket( AF_INET , SOCK_STREAM , 0 );
	if( _sv_fd < 0 )
		throw( init_err() );
	memset( (char*)&addr, sizeof(addr) , 0 );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( _port );
	addr.sin_addr.s_addr = INADDR_ANY;
	int rc = bind( _sv_fd , ( sockaddr_t* )&addr, sizeof( addr ) );
	if( rc < 0 )
		throw( init_err() );
	rc = listen( _sv_fd , BACK_LOG );
	if( rc < 0 )
		throw( init_err() );
}

void MapServer::start()
{
	void* p_self = reinterpret_cast< void* >( this );
	int rc = pthread_create( &_tid_main , NULL , MapServer::thread_main , p_self );
	if( rc )
		throw( start_err() );
		
}

void MapServer::stop()
{
	_exit_flag = true;
	void** p_ret_val;
	// 等待线程结束：
	pthread_join( _tid_main , p_ret_val );
}
// ------------ 服务函数 -------------------------------------
void MapServer::server_main()
{
	char cmd_buf[CMDBUF_LEN];
	memset( cmd_buf , CMDBUF_LEN , 0 );
	int new_fd = accept( _sv_fd , (sockaddr_t*)&cl_addr , &cl_size );
	cout<<__FILE__<<":"<<__LINE__<<" new_fd ="<<new_fd<<endl;
	// 正确性检查：
	if( new_fd < 0 )
		return;
	// 接受请求命令：
	size_t pos = 0;
	recv( new_fd , cmd_buf , CMDBUF_LEN , 0 );
	/*
	for( ; ; )
	{
		char* p_recv = &cmd_buf[ pos ];
		size_t num_rd = recv( new_fd , p_recv , CMDBUF_LEN - pos , 0 );
		cout<<__FILE__<<":"<<__LINE__<<"num_rd="<<num_rd<<endl;
		if( 0 == num_rd )
			break;
		pos += num_rd;
		// 越界处理：
		if( pos >= CMDBUF_LEN )
		{
			cout<<__FILE__<<":"<<__LINE__<<"pos="<<pos<<endl;
			return;
		}
	}	*/
	// 解析命令：
	request_t* p_rq = reinterpret_cast< request_t* >( cmd_buf );
	cout<<__FILE__<<":"<<__LINE__<<"Map type:"<<p_rq->map_type<<endl;
	/////+++++ 将来添加命令id解析+++++++
	// 根据地图类型，发送不同数据：
	switch( p_rq->map_type )
	{
		case ID_TEST:
			ack_msg( new_fd , "Test OK!" );
			break;
		case ID_CVGRID_MAP:
			cout<<__FILE__<<":"<<__LINE__<<" Client request cvmap!"<<endl;
			ack_cvmap( new_fd );
			break;
		case ID_DSGRID_MAP:
			ack_dsmap( new_fd );
			break;
		default:
			ack_msg( new_fd , "Unknow operation!" );
	}
	close( new_fd );
}
// 设置地图
void MapServer::operator<<( HIMMGrid& r_hg )
{
	p_cmap = &r_hg;
}

void MapServer::operator<<( DistanceMap& r_dm )
{
	p_dmap = &r_dm;
}

// 回应信息：
void MapServer::ack_msg( int fd , const string& msg )
{
	char* p_msg = new char[msg.size()+1];
	p_msg[0] = ID_STRING;
	memcpy( &p_msg[1] , msg.c_str() , msg.size() );
	send_data( fd , p_msg , msg.size()+1 );
}

void MapServer::ack_cvmap( int fd )
{
	if( p_cmap )
	{
		HIMMGrid2Char hg2char;
		hg2char.set_map( *p_cmap , ID_CVGRID_MAP );
		send_data( fd , hg2char.data() , hg2char.size() );
	}
	else
		ack_msg( fd , "Null cvmap pointer!" );
}

void MapServer::ack_dsmap( int fd )
{
	if( p_dmap )
	{
		DistanceGrid2Char dg2char;
		dg2char.set_map( *p_dmap , ID_DSGRID_MAP );
		send_data( fd , dg2char.data() , dg2char.size() );
	}
	else
		ack_msg( fd , "Null dsmap pointer!" );
}

void MapServer::send_data( int fd , char* pd , size_t num )
{
	size_t pos = 0;
	// 发送完所有数据
	for( ; ; )
	{
		size_t sd_num = send( fd , &pd[pos] , num - pos ,0);
		pos += sd_num;
		if( pos >= num )
			break;
	}
}

// ------------ 实现不同功能的线程函数 -----------------------
// 主线程函数：
void* MapServer::thread_main( void* p_self )
{
	MapServer* p_sv = reinterpret_cast< MapServer* >( p_self );
	while( !p_sv->_exit_flag )
		p_sv->server_main( );
}
	
