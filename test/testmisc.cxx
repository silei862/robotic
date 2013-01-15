/*
 * =====================================================================================
 *
 *       Filename:  testmisc.cxx
 *
 *    Description:  misc库测试程序
 *
 *        Version:  1.0
 *        Created:  2013年01月15日 10时38分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

#include <misc.h>
using namespace SlamLab;

int main()
{
	size_t distrib[60]={0};
	srand(time(NULL));
	for( int i = 0 ; i < 100000 ; i++)
	{
		double val = 25+10*randnormal(0,0.5);
		if( val >=0 && val < 60 )
			distrib[int(val)]++;	
		else
			cout<<"val="<<val<<endl;
	}
	for( int i = 0 ; i < 60 ; i++ )
		cout<<"["<<i<<"]="<<distrib[i]<<endl;
	return EXIT_SUCCESS;
}
