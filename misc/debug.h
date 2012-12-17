/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  调试用宏和函数，包含断言（ASSERT )、调试信息、警告、错误三个级别
 *    				的调试信息输出控制。和异常类一起使用
 *
 *        Version:  1.0
 *        Created:  2012年06月26日 10时32分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (SiLei), silei862@gmail.com
 *   Organization:  TSA.PLA
 *
 * =====================================================================================
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdlib.h>
#include <iostream>
#include <string>
#include "exception.h"

// 字符串转换宏:
#define _TO_STR( a ) #a

// 条件断言，如果为假则抛出异常
//#define _ASSERT( con )																\
//	if ( !con )																		\
//		throw(Exception(std::string("Assert Failure:")+std::string(_TO_STR(con)),	\
//			 			Exception::EXID_ASSERTFAIL,									\
//						std::string(__FILE__),										\
//						__LINE__));	

// 如果条件不满足，则终止程序执行
#define _ASSERT( con ) 		\
	if( !(con) ){			\
		std::cerr<<"Error:"<<__FILE__<<"["<<__LINE__<<"] assert failure: "<<_TO_STR( con )<<std::endl;	\
		exit(EXIT_FAILURE);	}


// 调试编译开关
#ifdef DEBUG
	#define ASSERT( con ) _ASSERT( con )
#else 	//_DEBUG_H_
	#define ASSERT( con )
#endif

// 信息输出宏
#define _DBG_PRINT( title,msg ) std::cout<<"["<<title<<"]"<<__FILE__<<":"<<__LINE__<<" "<<msg<<std::endl;
#define _DBG_USER_PRT(title,msg,user_func)	\
		_DBG_PRINT( title,msg )				\
		user_func;
#define _DBG_VAR_PRINT( var ) std::cout<<__FILE__<<"["<<__LINE__<<"]:"<<std::string(_TO_STR(var))<<"="<<var<<std::endl;
// 自由语句宏
#define _DBG_STATEMENT( usr_func ) usr_func;


// 信息输出开关
#ifdef INFO_ON
	#define DBG_INFO(msg) _DBG_PRINT("Info",msg);
	#define DBG_USER_INFO(msg,user_func) _DBG_USER_PRT("Info",msg,user_func)
	#define DBG_RUN( user_func ) _DBG_STATEMENT( user_func )
	#define INFO_VAR( var ) _DBG_VAR_PRINT( var )
#else	//INFO_ON
	#define DBG_INFO(msg)
	#define DBG_USER_INFO( msg, user_func )
	#define DBG_RUN( user_func )
	#define INFO_VAR( var )
#endif	//INFO_ON 

// 警告输出开关
#ifdef WARN_ON
	#define DBG_WARN(msg) _DBG_PRINT("Warning",msg)
	#define DBG_USER_WARN( msg,user_func ) _DBG_USER_PRT( "Warning",msg,user_func )
#else	//WARN_ON
	#define DBG_WARN(msg)
	#define DBG_USER( msg,user_func )
#endif	//WARN_ON

// 错误输出开关
#ifdef ERROR_ON
	#define DBG_ERROR( msg ) _DBG_PRINT("Error",msg)
	#define DBG_USER_ERROR( msg,user_func ) _DBG_USER_PRT( "Error",msg,user_func )
#else	//ERROR_ON
	#define DBG_ERROR( msg )
	#define DEB_USER_ERROR( msg,user_func )
#endif	//ERROR_ON

#endif	//_DEBUG_H_
