/*
 * =====================================================================================
 *
 *       Filename:  exception.cxx
 *
 *    Description:  异常类相关函数
 *
 *        Version:  1.0
 *        Created:  2012年12月31日 21时53分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <exception.h>
using namespace std;

ostream& std::operator<<( ostream& r_os , SlamLab::Exception& r_ex )
{
	r_os<<"[Exception]"<<r_ex.file()<<"["<<r_ex.line()<<"]:"<<r_ex.info()<<endl;
	return r_os;
}
