/*
 * =====================================================================================
 *
 *       Filename:  mapserver.h
 *
 *    Description:  地图数据服务器
 *
 *        Version:  1.0
 *        Created:  2012年12月25日 11时11分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _MAPSERVER_H_
#define _MAPSERVER_H_

#include <pthread.h>
#include <stdint.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <himmgrid.h>
#include <distancegrid.h>

namespace SlamLab
{
	// 常量
	enum _CONST
	{
		DEF_PORT 	= 8126,
		BACK_LOG 	= 1,
		CMDBUF_LEN	= 256,
	};
	// 命令类型标识
	enum _CMD_ID
	{
		ID_MAPTEST	= 0,
		ID_GETMAP	= 1,
	};
	// 数据类型标识
	enum _DATA_ID
	{
		ID_TEST			= 0,
		ID_STRING		= 1,
		ID_CVGRID_MAP	= 2,
		ID_DSGRID_MAP	= 3,
	};
	// 控制结构
	struct request_t
	{
		uint8_t cmd_id;
		uint32_t map_type;
	};

	class MapServer
	{
		// ------------------- 内部使用类型 ----------------
		typedef struct sockaddr_in sockaddrin_t;
		typedef struct sockaddr	sockaddr_t;
		// ------------------- 内嵌异常类 ------------------
		public:
		class init_err{ };
		class start_err{ };

		public:
			MapServer( uint16_t port = DEF_PORT );
			virtual ~MapServer(){ }
		public:
			// -------------- 服务器控制 ---------------
			void init();
			void start();
			void stop();
			// 服务函数
			void server_main( );
			// 设置地图
			void operator<<( HIMMGrid& r_hg );
			void operator<<( DistanceMap& r_dm );
		private:
			// 功能函数：
			void ack_msg( int fd , const std::string& msg );
			void ack_cvmap( int fd );
			void ack_dsmap( int fd );
			// 传送函数
			void send_data( int fd , char* pd , size_t num );
			// 服务线程 
			static void* thread_main( void* p_self );

		private:
			HIMMGrid*	 	p_cmap;
			DistanceMap*	p_dmap;
			// 服务器地址、端口、套接字描述符
			uint16_t		_port;
			sockaddrin_t 	addr;
			int				_sv_fd;
			// 客户端地址
			sockaddrin_t	cl_addr;
			socklen_t		cl_size;
			// 线程管理相关：
			bool			_exit_flag;
			pthread_t		_tid_main;
	};
}

#endif //_MAPSERVER_H_
