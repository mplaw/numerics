#pragma once

//	A set of useful macros for debugging and compiler-independent optimisations.
//	Optimisations that are not supported by a compiler will be ignored.
//
//	DEBUG
//		LOG(x) : prints out __FILE__:__LINE__:x
//		ADD(x) : adds x in if debug is 1, otherwise removes x
//		REM(x) : removes x if debug is 1, otherwise adds x in
//		EQ(x,y): asserts x == y
//		NE(x,y): asserts x != y
//		LT(x,y): asserts x <  y
//		GT(x,y): asserts x >  y
//		LE(x,y): asserts x <= y
//		GE(x,y): asserts x >= y
//
//	COMPILER				
//		COMMAND		: DESCRIPTION								: USE				|  GCC 		| MSVC		| Clang 	| Intel 	|
//		=========================================================================================================================================================================================
//		  LIKELY(x) 	: Tells the compiler to assume predict the true branch of x		:				|	y	|	n	|		|		|
//		UNLIKELY(x) 	: Tells the compiler to assume predict the false branch of x		:				|	y	|	n	|		|		|
//		RESTRICT    	: Tells the compiler that the pointer is unique.			: before pointer var(s)		|		|	y	|		|		|
//		RESTRICT_ALL	: Returned pointer will not be aliased by other pointers		: before function's return type	|		|	y	|		|		|
//		NO_ALIAS	: Does not interefere with global state (except via pointers)		: before function		|		|	y	|		|		|
//		NO_INLINE	: Don't inline the function						: before function		|		|	y	|		|		|
//		SPOINTER	: When converting from a 32 to 64 bit pointer, set new bits to F	:				|		|	y	|		|		|		
//		UPOINTER	: When converting from a 32 to 64 bit pointer, set new bits to 0	:				|		|	y	|		|		|
//		ALIGN		: Precisely control the alignment of user-defined data			: before struct/class/union/var	|		|	y	|		|		|
//		NAKED		: Don't generate prologoue or epilogue code for function		: before function		|		|	y	|		|		| only available on x86 and ARM
//		NO_VTABLE	: Don't generate v_table for class					: before class			|		|	y	|		|		|
//		NO_BUFFER_SAFETY: Turns off automatic buffer boundary checking				:				|		|	y	|		|		|		
//		SELECT_ANY	: Declared global data item (var or object) is a pick-any COMDAT: before var/object			|		|	y	|		|		|
//		THREAD_LOCAL: Declare a thread local variable.						: before var/object		|		|	y	|		|		|
//		UUID		:									:				|		|	y	|		|		|

//	TODO
//		- add all other MSVC keywords from https://msdn.microsoft.com/en-us/library/dabb5z75.aspx
//		- start other compilers
//		- consider deleting short hands and bad spelling
//		- finish documentation
//


#include "array"
#include <iostream>
#include <string>
#include <assert.h>

// Flags
//------------------------------------------------------------------------------------

#define DEBUG			1
#define COMPILER  		1	// (0:GCC), (1:MSCV), (2:CLANG), (3:Intel)
#define SHORTHANDS		0

// Macros
//------------------------------------------------------------------------------------

#if DEBUG
	#define EQ(x, y) assert((x) == (y))
	#define NE(x, y) assert((x) != (y))
	#define LT(x, y) assert((x) <  (y))
	#define GT(x, y) assert((x) >  (y))
	#define LE(x, y) assert((x) <= (y))
	#define GE(x, y) assert((x) >= (y))
	#define ADD(x) x
	#define REM(x)
	#define LOG(x) std::cout <<  __FILE__ << ":" << __LINE__  << (x) << std::endl;
#else
	#define EQ(x, y)
	#define NE(x, y)
	#define LT(x, y)
	#define GT(x, y)
	#define LE(x, y)
	#define GE(x, y)
	#define ADD(x) 
	#define REM(x) x
	#define LOG(x) 
#endif


#if   COMPILER == 0		// GCC
	#define   LIKELY(expression)	(__builtin_expect((expression), 1))
	#define UNLIKELY(expression)	(__builtin_expect((expression), 0))
	#define RESTRICT __restrict__
	#define RESTRICT_ALL __declspec(restrict)
	#define SPOINTER __sptr
	#define UPOINTER __uptr

#elif COMPILER == 1		// MSVC				// additional predefined macros https://msdn.microsoft.com/en-us/library/b0084kay.aspx
	#define   LIKELY(expression)
	#define UNLIKELY(expression)
	#define RESTRICT __restrict				// https://msdn.microsoft.com/en-us/library/5ft82fed.aspx
	#define RESTRICT_ALL() __declspec ( restrict )		// see above or https://msdn.microsoft.com/en-us/library/8bcxafdh.aspx
	#define SPOINTER __sptr					// https://msdn.microsoft.com/en-us/library/aa983399.aspx
	#define UPOINTER __uptr					// see above
	#define UNALIGNED  __unaligned				// https://msdn.microsoft.com/en-us/library/ms177389.aspx 
	#define ALIGN(x)  __declspec(align(x))			// https://msdn.microsoft.com/en-us/library/83ythb65.aspx
	#define NO_ALIAS  __declspec(noalias)			// https://msdn.microsoft.com/en-us/library/k649tyc7.aspx
	#define NO_INLINE __declspec(noinline)			// https://msdn.microsoft.com/en-us/library/kxybs02x.aspx
	#define NO_RETURN __declspec(noreturn)			// https://msdn.microsoft.com/en-us/library/k6ktzx3s.aspx
	#define NAKED	  __declspec(naked)			// https://msdn.microsoft.com/en-us/library/h5w10wxs.aspx
	#define NO_VTABLE __declspec(novtable)			// https://msdn.microsoft.com/en-us/library/k13k85ky.aspx
	#define PROCESS   __declspec(process)			//
	#define PROPERTY(get_func_name, put_func_name) __declspec( property( get=get_func_name, put=put_func_name ) )
	#define PROPERTY_GET(get_func_name) __declspec( property( get=get_func_name ) )
	#define PROPERTY_PUT(put_func_name) __declspec( property( put=put_func_name ) )
	#define NO_BUFFER_SAFETY_CHECKS __declspec( safebuffers )
	#define SELECT_ANY __declspec( selectany )		//
	#define THREAD_LOCAL __declspec( thread )		//
	#define UUID(x) __declspec( uuid(x) )			// https://msdn.microsoft.com/en-us/library/3b6wkewa.aspx
	//#define ALLOCATE(pragma_dec, segname) #pragma pragma_dec(segname, read) __declspec(allocate(x)) // https://msdn.microsoft.com/en-us/library/5bkb2w6t.aspx


#elif COMPILER == 2		// CLANG
	#define   LIKELY(expression)
	#define UNLIKELY(expression)
	#define RESTRICT __restrict__
	#define RESTRICT_ALL()
	#define SPOINTER
	#define UPOINTER
	#define UNALIGNED 
	#define ALIGN(x)
	#define ALLOCATE(pragma_dec, segname)
	#define NO_ALIAS
	#define NO_INLINE
	#define NO_RETURN
	#define NAKED	  
	#define NO_VTABLE

#else					// UNKNOWN
	#define   LIKELY(expression)
	#define UNLIKELY(expression)
	#define RESTRICT
	#define RESTRICT_ALL()
	#define SPOINTER __sptr
	#define UPOINTER __uptr

#endif


#if SHORTHANDS
	#define MIN(x,y) ((x)<(y)?(x):(y))
	#define MAX(x,y) ((x)>(y)?(x):(y))
	#define SIGN(x) ((x)>0?1:((x)<0?-1:0))
	#define PRINT(x) {cout<<#x<<": "<<x;}
#else
	#define MIN(x,y)
	#define MAX(x,y)
	#define SIGN(x)
	#define PRINT(x) {cout<<#x<<": "<<x;}
#endif

