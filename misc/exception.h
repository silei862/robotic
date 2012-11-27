/*
 * =====================================================================================
 *
 *       Filename:  exception.h
 *
 *    Description:  异常处理类,提供和传递异常发生时的现场信息
 *
 *        Version:  1.0
 *        Created:  2012年06月26日 10时05分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <stdint.h>

#define INFO_UNKNOWN "unknown"

#ifndef __LINE__
#define __LINE__ 0
#endif //__LINE__

#ifndef __FILE__
#define __FILE__ "File_Unkown"
#endif //_FILE__

namespace SlamLab
{
	enum _EXCEP_ID{
			EXID_UNKNOWN 	= 0,
			EXID_ASSERTFAIL = 1,
			EXID_NULLPOINT	= 2,
			EXID_FILE_ERROR	= 3,
		};

	class Exception
	{
		public:
		/* -------------- Life Cycle --------------- */
		public:
			Exception()
				:info( INFO_UNKNOWN ),id( EXID_UNKNOWN ),line( 0 ),file( INFO_UNKNOWN ) { }

			Exception(const char* info,uint32_t id,const char* file,uint32_t line)
				:info( info ),id( id ),file( file ),line( line ){ }

			Exception(const std::string& info,uint32_t id,const std::string& file,uint32_t line)
				:info( info ),id( id ),file( file ),line( line ){ }

			Exception( const Exception& e )
				:info( e.info ),id( e.id ),file( e.file ),line( e.line ){ }

		/* ---------------- Acessor --------------- */
			uint32_t get_id() { return id;}
			const std::string& get_info() { return info;}
			const std::string& get_file() { return file;}
			uint32_t get_line() { return line;}
		private:
			std::string info;
			uint32_t id;
			uint32_t line;
			std::string file;
	};
}

#endif	//_EXCEPTION_H_
