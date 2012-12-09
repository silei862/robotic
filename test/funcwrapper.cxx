/*
 * =====================================================================================
 *
 *       Filename:  funcwrapper.cxx
 *
 *    Description:  函数包装器测试
 *
 *        Version:  1.0
 *        Created:  2012年12月10日 00时25分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Lei.Si), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <funcwrapper.h>
using namespace SlamLab;

class Ball
{
	public:
		void intro()
		{
			cout<<"I am Ball!"<<endl;
		}
		int get_id()
		{
			return 9527;
		}
		int index( int i )
		{
			return i*2+1;
		}
		int add( int a , int b )
		{
			return a+b;
		}

};

float pow( float a ,int x )
{
	float pw = 1;
	for( int i =0 ; i < x ; i++ )
		pw*=a;
	return pw;
}

int main()
{
	Ball sball;
	MFuncWrapper< void( Ball ) > intro( &sball, &Ball::intro );
	intro();

	MFuncWrapper< int( Ball ) > get_id( &sball, &Ball::get_id );
	cout<<"get_id()="<<get_id()<<endl;

	MFuncWrapper< int( Ball, int ) > index( &sball , &Ball::index );
	cout<<"index(5)="<<index(5)<<endl;

	MFuncWrapper< int( Ball, int , int ) > add( &sball , &Ball::add );
	cout<<"add( 2 , 3 )="<<add( 2 , 3 )<<endl;
	
	FuncWrapper< float( float , int ) > wpow( &pow );
	cout<<"wpow( 2 , 8 )="<<wpow( 2.0 ,8 )<<endl;

	return EXIT_SUCCESS;
}

