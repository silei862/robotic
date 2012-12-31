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
#include <iostream>

#define INFO_UNKNOWN "unknown"

#ifndef __LINE__
#define __LINE__ 0
#endif //__LINE__

#ifndef __FILE__
#define __FILE__ "File_Unkown"
#endif //_FILE__

namespace SlamLab
{

	class Exception
	{
		public:
		/* -------------- Life Cycle --------------- */
		public:
			Exception()
				:_info( INFO_UNKNOWN ),_line( 0 ),_file( INFO_UNKNOWN ) { }

			Exception(const char* info,const char* file,uint32_t line)
				:_info( info ),_file( file ),_line( line ){ }

			Exception(const std::string& info,const std::string& file,uint32_t line)
				:_info( info ),_file( file ),_line( line ){ }

			Exception( const Exception& e )
				:_info( e._info ),_file( e._file ),_line( e._line ){ }

		/* ---------------- Acessor --------------- */
			const std::string& info() { return _info;}
			const std::string& file() { return _file;}
			uint32_t line() { return _line;}
		private:
			std::string _info;
			uint32_t _line;
			std::string _file;
	};
}

#define throw_info( info ) throw( Exception( info , __FILE__ ,__LINE__));
// 异常信息输出函数：
namespace std
{
	ostream& operator<<( ostream& r_os , SlamLab::Exception& r_ex );
}
#endif	//_EXCEPTION_H_
