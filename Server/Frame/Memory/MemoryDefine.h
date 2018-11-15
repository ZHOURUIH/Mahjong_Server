#ifndef _MEMORY_DEFINE_H_
#define _MEMORY_DEFINE_H_

#include "ServerDefine.h"
#include "GameLog.h"
#include "StringUtility.h"

#ifdef CHECK_MEMORY
// 带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)	\
NULL;									\
try										\
{										\
	ptr = new className(__VA_ARGS__);	\
	txMemoryCheck::usePtr(ptr);			\
}										\
catch (...)								\
{										\
	ptr = NULL;							\
	LOG_ERROR(string("can not alloc memory! ") + \
	"className : " + TOSTRING(className));	\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)	\
NULL;											\
try												\
{												\
	ptr = new className[count];					\
	memset(ptr, 0, sizeof(className)* count);	\
	txMemoryCheck::usePtr(ptr);					\
}												\
catch (...)										\
{												\
	ptr = NULL;									\
	LOG_ERROR(string("can not alloc memory array! ") + \
	"className : " + TOSTRING(className) + \
	", count : " + StringUtility::intToString(count));	\
}

#define NORMAL_DELETE(ptr)			\
if (ptr != NULL)					\
{									\
	txMemoryCheck::unusePtr(ptr);	\
	delete ptr;						\
	ptr = NULL;						\
}

#define NORMAL_DELETE_ARRAY(ptr)	\
if (ptr != NULL)					\
{									\
	txMemoryCheck::unusePtr(ptr);	\
	delete[] ptr;					\
	ptr = NULL;						\
}
#else
// 不带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)	\
NULL;									\
try										\
{										\
	ptr = new className(__VA_ARGS__);	\
}										\
catch (...)								\
{										\
	ptr = NULL;							\
	LOG_ERROR(string("can not alloc memory! ") + \
	"className : " + TOSTRING(className));	\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)	\
NULL;											\
try												\
{												\
	ptr = new className[count];					\
	memset(ptr, 0, sizeof(className) * count);	\
}												\
catch (...)										\
{												\
	ptr = NULL;									\
	LOG_ERROR(string("can not alloc memory array! ") + \
	"className : " + TOSTRING(className) + \
	", count : " + StringUtility::intToString(count));	\
}

#define NORMAL_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

#define NORMAL_DELETE_ARRAY(ptr)\
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}
#endif

#ifdef TRACE_MEMORY
// 申请无参或者带参构造类的内存
#define TRACE_NEW(className, ptr, ...)		\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// 申请无参构造的类或者基础数据类型数组内存
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// 释放TRACE_NEW申请的内存
#define TRACE_DELETE(ptr)			\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// 释放TRACE_NEW_ARRAY申请的内存
#define TRACE_DELETE_ARRAY(ptr)		\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE_ARRAY(ptr)
#else
#define TRACE_NEW(className, ptr, ...)			NORMAL_NEW(className, ptr, __VA_ARGS__)
#define TRACE_NEW_ARRAY(className, count, ptr)  NORMAL_NEW_ARRAY(className, count, ptr)
#define TRACE_DELETE(ptr)						NORMAL_DELETE(ptr)
#define TRACE_DELETE_ARRAY(ptr)					NORMAL_DELETE_ARRAY(ptr)
#endif

#endif