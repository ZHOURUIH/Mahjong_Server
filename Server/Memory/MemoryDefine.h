#ifndef _MEMORY_DEFINE_H_
#define _MEMORY_DEFINE_H_

//#define TRACE_MEMORY
//#define CHECK_MEMORY

#include "ServerDefine.h"
#include "GameLog.h"
#include "txMemoryCheck.h"

// ����ʲôƽ̨����Ҫ���������������ڴ������
#define NEW_MEMORY(className, ptr, check, ...)		\
NULL;												\
try													\
{													\
	ptr = new className(__VA_ARGS__);				\
	if (check)										\
	{												\
		txMemoryCheck::usePtr(ptr);					\
	}												\
}													\
catch (...)											\
{													\
	ptr = NULL;										\
	GAME_ERROR("error : can not alloc memory!");	\
}

// ���������������ڴ�
#define NEW_MEMORY_ARRAY(className, count, ptr, check)		\
NULL;														\
if (count <= 0)												\
{															\
	ptr = NULL;												\
}															\
else														\
{															\
	try														\
	{														\
		ptr = new className[count];							\
		memset(ptr, 0, sizeof(className)* count);			\
		if (check)											\
		{													\
			txMemoryCheck::usePtr(ptr);						\
		}													\
	}														\
	catch (...)												\
	{														\
		ptr = NULL;											\
		GAME_ERROR("error : can not alloc memory array!"); \
	}														\
}															\

// �������ͷ��ڴ�
#define DELETE_MEMORY(ptr, check)		\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete ptr;							\
	ptr = NULL;							\
}

// �������ͷ������ڴ�
#define DELETE_MEMORY_ARRAY(ptr, check)	\
if (ptr != NULL)						\
{										\
	if (check)							\
	{									\
		txMemoryCheck::unusePtr(ptr);	\
	}									\
	delete[] ptr;						\
	ptr = NULL;							\
}

#ifdef CHECK_MEMORY
// ���ڴ�Ϸ����ĳ����ڴ�������ͷ�
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, true, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, true)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, true)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, true)
#else
// �����ڴ�Ϸ����ĳ����ڴ�������ͷ�
#define NORMAL_NEW(className, ptr, ...)			NEW_MEMORY(className, ptr, false, __VA_ARGS__)
#define NORMAL_NEW_ARRAY(className, count, ptr)	NEW_MEMORY_ARRAY(className, count, ptr, false)
#define NORMAL_DELETE(ptr)						DELETE_MEMORY(ptr, false)
#define NORMAL_DELETE_ARRAY(ptr)				DELETE_MEMORY_ARRAY(ptr, false)
#endif

#ifdef TRACE_MEMORY
// �����޲λ��ߴ��ι�������ڴ�
#define TRACE_NEW(className, ptr, ...)		\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// �����޲ι��������߻����������������ڴ�
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// �ͷ�TRACE_NEW������ڴ�
#define TRACE_DELETE(ptr)			\
txMemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// �ͷ�TRACE_NEW_ARRAY������ڴ�
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