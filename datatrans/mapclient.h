/*
 * =====================================================================================
 *
 *       Filename:  mapclient.h
 *
 *    Description:  地图数据接收客户端
 *
 *        Version:  1.0
 *        Created:  2012年12月25日 11时31分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _MAPCLIENT_H_
#define _MAPCLIENT_H_

#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string>
#include <himmgrid.h>
#include <distancegrid.h>
#include <mapserver.h>

#define DEF_ADDR "127.0.0.1"

namespace SlamLab
{
	class MapClient
	{
		enum _CONST
		{
			RBUF_SIZE = 1024*64,
		};
		typedef struct sockaddr_in sockaddrin_t;
		public:
			class setuplink_err{ };
			class data_err{ };
			class alloc_fail{ };
		public:
			MapClient( const std::string& addr = DEF_ADDR , uint16_t port = DEF_PORT );
			virtual ~MapClient(){ }

		public:
			// 对外接口：
			HIMMGrid& operator>>( HIMMGrid& r_hg );
			DistanceMap& operator>>( DistanceMap& r_dm );
			MapClient& operator()( const std::string& addr , uint16_t port = DEF_PORT );
			const std::string& server_ret();
		private:
			// 内部操作：
			void _setup_link();
			void _send_data( char* p_data , size_t num );
			void _recv_data( char*& p_data , size_t& num );

		private:
			int				_fd;
			sockaddrin_t	_sv_addr;	
			std::string		_ip_addr;
			uint16_t		_port;
			std::string		_sv_retstr;
	};
}


#endif //_MAPCLIENT_H_
