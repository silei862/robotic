/*
 * =====================================================================================
 *
 *       Filename:  dtest.cxx
 *
 *    Description:  debug中的宏测试
 *
 *        Version:  1.0
 *        Created:  2012年12月17日 21时56分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
using namespace std;

#define DEBUG
#include <debug.h>

int main()
{
	ASSERT( 3 < 2 );
	cout<<"Reach end successful!"<<endl;
	return EXIT_SUCCESS;
}
