#pragma once

#define SafeRelease(p)			{ if((p) != NULL) { (p)->Release(); (p)=NULL; } }
#define SafeDelete(p)			{ if((p) != NULL) { delete (p);     (p)=NULL; } }
#define SafeDelArray(p)			{ if((p) != NULL) { delete [](p);	(p) = NULL;} }

#define ArraySize(arrayname)	(sizeof(arrayname) / sizeof(arrayname[0]))

//----------------------------------------------------------------------------------------
#define MaxPathLen				256

/*
//----------------------------------------------------------------------------------------
#define DEBUG_MSG
#define ERROR_MSG
#define WARNING_MSG
#define TRACE_MSG
#define INFO_MSG

//----------------------------------------------------------------------------------------
#if defined(_DEBUG)
	#define MakeLibname(module)				#module".lib"
#else
	#define MakeLibname(module)				#module".lib"
#endif
//----------------------------------------------------------------------------------------

#define CHR(X, Y)  CHR_REAL(X, Y)
#define CHR_REAL(X, Y) X##Y

#define EXS(X) EXXS(X)
#define EX0(X) EXX0(X)
#define EX1(X) EXX1(X)
#define EX2(X) EXX2(X)
#define EX3(X) EXX3(X)
#define EX4(X) EXX4(X)
#define EX5(X) EXX5(X)
#define EX6(X) EXX6(X)
#define EX7(X) EXX7(X)
#define EX8(X) EXX8(X)
#define EX9(X) EXX9(X)

#define EXXS(X) X
#define EXX0(X) X
#define EXX1(X) X
#define EXX2(X) X
#define EXX3(X) X
#define EXX4(X) X
#define EXX5(X) X
#define EXX6(X) X
#define EXX7(X) X
#define EXX8(X) X
#define EXX9(X) X

#define CHAIN_S( F, X )		  EXS( CHAIN_0 F##_##X )
#define CHAIN_0( M, F, A, R ) EX0( M##_##F(1,A,R) )
#define CHAIN_1( M, F, A, R ) EX1( M##_##F(2,A,R) )
#define CHAIN_2( M, F, A, R ) EX2( M##_##F(3,A,R) )
#define CHAIN_3( M, F, A, R ) EX3( M##_##F(4,A,R) )
#define CHAIN_4( M, F, A, R ) EX4( M##_##F(5,A,R) )
#define CHAIN_5( M, F, A, R ) EX5( M##_##F(6,A,R) )
#define CHAIN_6( M, F, A, R ) EX6( M##_##F(7,A,R) )
#define CHAIN_7( M, F, A, R ) EX7( M##_##F(8,A,R) )
#define CHAIN_8( M, F, A, R ) EX8( M##_##F(9,A,R) )
#define CHAIN_9( M, F, A, R ) EX9( M##_##F(10,A,R) )

#define GET_THIS_BASS(X)  CHAIN_S(AUTO_WRITE_PARSE, X)

#define AUTO_WRITE_PARSE_BASS(T, R)		(AUTO_WRITE_DO,BASS,T,R)

#define AUTO_WRITE_DO_BASS(N, A, R)									\
	AUTO_WRITE_STD(N,A)												\
	CHR(CHAIN_##N, AUTO_WRITE_PARSE_##R)							\


#define AUTO_WRITE_PARSE_END			(AUTO_WRITE_DO,END,M,M)	
#define AUTO_WRITE_DO_END(N,A,R)		return NULL;

#define AUTO_WRITE_STD(N,T)											\
	ptrThis = T##::_getThis(className);								\
	if (ptrThis != NULL)											\
		return ptrThis;												\


#define DEFINE_GET_THIS(driveClass, baseClass)						\
	virtual void* GetThis(const char* className = NULL) {			\
		if(className == NULL)										\
			return this;											\
		return driveClass::_getThis(className);						\
	}																\
	void* _getThis(const char* className) {							\
		if(strcmp(className, driveClass::GetModuleName()) == 0)		\
			return this;											\
		void* ptrThis = NULL;										\
		GET_THIS_BASS(baseClass)									\
	}																\

//----------------------------------------------------------------------------------------

#define MACRO_TO_STRING(N) MACRO_TO_STRING_REAL(N)
#define MACRO_TO_STRING_REAL(N) #N

//----------------------------------------------------------------------------------------
*/