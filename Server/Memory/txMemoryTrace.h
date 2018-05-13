﻿#ifndef _TX_MEMORY_TRACE_H_
#define _TX_MEMORY_TRACE_H_

#include "ServerDefine.h"
#include "ThreadLock.h"
#include "MemoryInfo.h"
#include "CustomThread.h"
#include "MemoryDefine.h"

#ifdef TRACE_MEMORY
#include "txShareMemoryServer.h"
// 注意事项!!!
// 需要确保MemeryTrace对象只有一个
// 所有的堆内存申请和释放都必须使用TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW申请的内存只能用TRACE_DELETE释放,TRACE_NEW_ARRAY申请的内存只能用TRACE_DELETE_ARRAY释放
// 虽然可以像new和delete一样的语法使用TRACE相关宏,但是不能直接在return后面加TRACE相关宏
// TRACE相关宏也不能在构造函数初始化列表中使用
const int MAX_COUNT = 1000;
class txMemoryTrace
{
public:
	txMemoryTrace();
	virtual ~txMemoryTrace();
	// writeOrDebug为真则表示将信息写入共享内存,然后使用MemeryViewer查看,为假时表示在控制台显示内存信息
	void init(const bool& writeOrDebug);
	static bool debugMemoryTrace(void* args);
	static bool writeMemoryTrace(void* args);
	static void insertPtr(void* ptr, MemoryInfo& info);
	static void erasePtr(void* ptr);
	static void setIgnoreClass(txSet<std::string>& classList){mIgnoreClass = classList;}
	static void setIgnoreClassKeyword(txSet<std::string>& classList){mIgnoreClassKeyword = classList;}
	static void setShowOnlyDetailClass(txSet<std::string>& classList){mShowOnlyDetailClass = classList;}
	static void setShowOnlyStatisticsClass(txSet<std::string>& classList){mShowOnlyStatisticsClass = classList;}
	static void setShowDetail(const bool& show){ mShowDetail = show; }
	static void setShowStatistics(const bool& show){ mShowStatistics = show; }
	static void setShowAll(const bool& show){ mShowAll = show; }
protected:
	// 内存申请总信息表
	static txMap<void*, MemoryInfo> mMemoryInfo;
	// 内存统计信息表, first是类型名,second的first是该类型名的内存个数,second是该类型占得总内存大小,单位是字节
	static txMap<std::string, MemoryType> mMemoryType;
	// 不显示该列表中类型的内存详细信息以及统计信息
	static txSet<std::string> mIgnoreClass;
	// 如果详细信息以及统计信息中的类型包含该列表中的关键字,则不显示
	static txSet<std::string> mIgnoreClassKeyword;
	// 只显示该列表中类型的内存详细信息,如果该列表为空,则全部显示
	static txSet<std::string> mShowOnlyDetailClass;
	// 只显示该列表中类型的内存统计信息,如果该列表为空,则全部显示
	static txSet<std::string> mShowOnlyStatisticsClass;
	// 是否显示总信息表的详细内容
	static bool mShowDetail;
	// 是否显示内存统计信息
	static bool mShowStatistics;
	// 是否显示内存总个数
	static bool mShowTotalCount;
	static bool mShowAll;
	static bool mWriteOrDebug;	// 为true表示将内存信息写入共享内存,false表示仅打印到控制台
	static ThreadLock mInfoLock;
	static CustomThread* mThread;
	static txShareMemoryServer* mShareMemoryServer;
	static txMap<std::string, int> mMemoryTypeIndex;
	static MemoryType mMemoryList[MAX_COUNT];
	static int mMemoryCount;
};
#endif

#endif