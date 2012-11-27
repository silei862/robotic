/*
 * =====================================================================================
 *
 *       Filename:  func_wrapper.h
 *
 *    Description:  函数包装器
 *
 *        Version:  1.0
 *        Created:  2012年11月25日 14时22分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  思磊 (Silei), silei862@gmail.com
 *   Organization:  TSA@PLA
 *
 * =====================================================================================
 */
#ifndef _FUNC_WRAPPER_H_
#define _FUNC_WRAPPER_H_

#include <stdlib.h>

namespace SlamLab
{
	template< typename TC, typename TR , typename TA >
		class MFuncWrapper
		{
			typedef TR(TC::*pFunc_t)( TA );
			
			public:
				MFuncWrapper( TC* p_obj , pFunc_t p_f )
					:p_object(p_obj),p_func( p_f ){ }

			public:
				void attach( TC* p_obj , pFunc_t p_f )
				{
					p_object = p_obj;
					p_func = p_f;
				}

				void clear() 
				{
					p_object = NULL;
					p_func = NULL;
				}

				TR operator()( TA para ){ return (p_object->*p_func)( para ); }

			private:
				TC* 		p_object;
				pFunc_t		p_func;
		};
}

#endif //_FUNC_WRAPPER_H_
