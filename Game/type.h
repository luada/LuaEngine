#pragma once


//@BeginExportToLua
typedef unsigned long		HandleType;

typedef unsigned char       BYTE;
typedef unsigned int        UINT;
typedef unsigned short		USHORT;

typedef unsigned int		UINT32;
typedef unsigned short		UINT16;
typedef unsigned char		UINT8;

typedef int					INT32;
typedef short				INT16;
//@EndExportToLua


#ifdef WIN32
#else  //Linux
typedef char				INT8;
typedef int64_t				INT64;
#endif


