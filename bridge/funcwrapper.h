/*
 * =====================================================================================
 *
 *       Filename:  funcwrapper.h
 *
 *    Description:  函数包装器
 *
 *        Version:  1.0
 *        Created:  2012年12月09日 22时44分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Lei.Si), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */

#ifndef _FUNCWRAPPER_H_
#define _FUNCWRAPPER_H_

#include <debug.h>

namespace SlamLab
{
	// ------------------- 普通函数部分 -------------------
	// 普通函数使用FuncWrapper包装
	template< typename FORM >
		class FuncWrapper;

	// 参数为0的模板
	template< typename RT >
		class FuncWrapper< RT() >
		{
			typedef RT(*FuncPtr_t)();
			public:
				FuncWrapper( FuncPtr_t p_fn )
					:p_func( p_fn ){ }
				// 实现Functor：
				RT operator()()
				{
					ASSERT( p_func );
					return (*p_func)(); 
				}
				// 实现赋值操作符
				void operator=( FuncPtr_t p_fn ){ p_func = p_fn; }
			private:
				FuncPtr_t p_func;
		};

	// 参数为1的模板
	template< typename RT , typename PA >
		class FuncWrapper< RT( PA ) >
		{
			typedef RT(*FuncPtr_t)( PA );
			public:
				FuncWrapper( FuncPtr_t p_fn )
					:p_func( p_fn ){ }
				//实现Functor:
				RT operator()( PA pa )
				{
					ASSERT( p_func );
					return (*p_func)(pa);
				}
				// 实现赋值操作：
				void operator=( FuncPtr_t p_fn ){ p_func = p_fn; }
			private:
				FuncPtr_t p_func;
		};
	
	// 参数为2的模板
	template< typename RT , typename PA , typename PB >
		class FuncWrapper< RT( PA , PB ) >
		{
			typedef RT(*FuncPtr_t)( PA, PB );
			public:
				FuncWrapper( FuncPtr_t p_fn )
					:p_func( p_fn ){ }
				//实现Functor:
				RT operator()( PA pa , PB pb )
				{
					ASSERT( p_func );
					return (*p_func)( pa , pb );
				}
				//实现赋值操作：
				void operator=( FuncPtr_t p_fn ){ p_func = p_fn; }
			private:
				FuncPtr_t p_func;
		};

	template< typename RT , typename PA , typename PB , typename PC >
		class FuncWrapper< RT( PA , PB ,PC ) >
		{
			typedef RT(*FuncPtr_t)( PA , PB , PC );
			public:
				FuncWrapper( FuncPtr_t p_fn )
					:p_func( p_fn ){ }
				//实现Functor:
				RT operator()( PA pa , PB pb , PC pc )
				{
					ASSERT( p_func );
					return (*p_func)( pa , pb , pc );
				}
				//实现赋值操作：
				void operator=( FuncPtr_t p_fn ){ p_func = p_fn; }
			private:
				FuncPtr_t p_func;		
		};

	// ------------------- 成员函数部分 --------------------
	// 成员函数使用MFuncWrapper模板包装
	template< typename FORM > 
		class MFuncWrapper;

	// 参数为0的模板：
	template< typename RT , typename CT >
		class MFuncWrapper< RT( CT ) >
		{
			typedef RT(CT::*FuncPtr_t)();
			public:
				MFuncWrapper( CT* p_ob , FuncPtr_t p_fn )
					:p_object( p_ob ),p_func( p_fn ){ }
				//链接一个成员函数
				void link( CT* p_ob , FuncPtr_t p_fn )
				{
					p_object = p_ob;
					p_func = p_fn;
				}
				//实现Functor:
				RT operator()()
				{
					ASSERT( p_object&&p_func );
					return ( p_object->*p_func )();
				}
			private:
				CT*			p_object;
				FuncPtr_t	p_func; 
		};

	// 参数为1的模板：
	template< typename RT , typename CT , typename PA >
		class MFuncWrapper< RT( CT , PA ) >
		{
			typedef RT(CT::*FuncPtr_t)( PA );
			public:
				MFuncWrapper( CT* p_ob , FuncPtr_t p_fn )
					:p_object( p_ob ), p_func( p_fn ){ }
				//链接一个成员函数：
				void link( CT* p_ob , FuncPtr_t p_fn )
				{
					p_object = p_ob;
					p_func = p_fn;
				}
				//实现Functor:
				RT operator()( PA pa )
				{
					ASSERT( p_object&&p_func );
					return  ( p_object->*p_func )( pa );
				}
			private:
				CT*			p_object;
				FuncPtr_t	p_func;
		};

	// 参数为2的模板
	template< typename RT , typename CT , typename PA , typename PB >
		class MFuncWrapper< RT( CT , PA , PB ) >
		{
			typedef RT(CT::*FuncPtr_t)( PA , PB );
			public:
				MFuncWrapper( CT* p_ob , FuncPtr_t p_fn )
					:p_object( p_ob ), p_func( p_fn ){ }
				//链接一个成员函数：
				void link( CT* p_ob , FuncPtr_t p_fn )
				{
					p_object = p_ob;
					p_func = p_fn;
				}
				//实现Functor:
				RT operator()( PA pa, PB pb )
				{
					ASSERT( p_object&&p_func );
					return  ( p_object->*p_func )( pa , pb );
				}
			private:
				CT*			p_object;
				FuncPtr_t	p_func;
		};
	template< typename RT , typename CT , typename PA , typename PB , typename PC >
		class MFuncWrapper< RT( CT , PA , PB , PC ) >
		{
			typedef RT(CT::*FuncPtr_t)( PA , PB , PC );
			public:
				MFuncWrapper( CT* p_ob , FuncPtr_t p_fn )
					:p_object( p_ob ) , p_func( p_fn ){ }
				//链接一个成员函数：
				void link( CT* p_ob , FuncPtr_t p_fn )
				{
					p_object = p_ob;
					p_func = p_fn;
				}
				//实现Functor:
				RT operator()( PA pa , PB pb , PC pc )
				{
					ASSERT( p_object&&p_func );
					return ( p_object->*p_func )( pa , pb , pc );
				}
			private:
				CT*			p_object;
				FuncPtr_t 	p_func;
		};
}
#endif //_FUNCWRAPPER_H_
