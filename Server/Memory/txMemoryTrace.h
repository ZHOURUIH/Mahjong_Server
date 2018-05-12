#ifndef _TX_MEMORY_TRACE_H_
#define _TX_MEMORY_TRACE_H_

#include "ServerDefine.h"
#include "ThreadLock.h"
#include "MemoryInfo.h"
#include "CustomThread.h"
#include "MemoryDefine.h"

#ifdef TRACE_MEMORY
#include "txShareMemoryServer.h"
// ע������!!!
// ��Ҫȷ��MemeryTrace����ֻ��һ��
// ���еĶ��ڴ�������ͷŶ�����ʹ��TRACE_NEW,TRACE_NEW_ARRAY,TRACE_DELETE,TRACE_DELETE_ARRAY
// TRACE_NEW������ڴ�ֻ����TRACE_DELETE�ͷ�,TRACE_NEW_ARRAY������ڴ�ֻ����TRACE_DELETE_ARRAY�ͷ�
// ��Ȼ������new��deleteһ�����﷨ʹ��TRACE��غ�,���ǲ���ֱ����return�����TRACE��غ�
// TRACE��غ�Ҳ�����ڹ��캯����ʼ���б���ʹ��
const int MAX_COUNT = 1000;
class txMemoryTrace
{
public:
	txMemoryTrace();
	virtual ~txMemoryTrace();
	// writeOrDebugΪ�����ʾ����Ϣд�빲���ڴ�,Ȼ��ʹ��MemeryViewer�鿴,Ϊ��ʱ��ʾ�ڿ���̨��ʾ�ڴ���Ϣ
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
	// �ڴ���������Ϣ��
	static txMap<void*, MemoryInfo> mMemoryInfo;
	// �ڴ�ͳ����Ϣ��, first��������,second��first�Ǹ����������ڴ����,second�Ǹ�����ռ�����ڴ��С,��λ���ֽ�
	static txMap<std::string, MemoryType> mMemoryType;
	// ����ʾ���б������͵��ڴ���ϸ��Ϣ�Լ�ͳ����Ϣ
	static txSet<std::string> mIgnoreClass;
	// �����ϸ��Ϣ�Լ�ͳ����Ϣ�е����Ͱ������б��еĹؼ���,����ʾ
	static txSet<std::string> mIgnoreClassKeyword;
	// ֻ��ʾ���б������͵��ڴ���ϸ��Ϣ,������б�Ϊ��,��ȫ����ʾ
	static txSet<std::string> mShowOnlyDetailClass;
	// ֻ��ʾ���б������͵��ڴ�ͳ����Ϣ,������б�Ϊ��,��ȫ����ʾ
	static txSet<std::string> mShowOnlyStatisticsClass;
	// �Ƿ���ʾ����Ϣ�����ϸ����
	static bool mShowDetail;
	// �Ƿ���ʾ�ڴ�ͳ����Ϣ
	static bool mShowStatistics;
	// �Ƿ���ʾ�ڴ��ܸ���
	static bool mShowTotalCount;
	static bool mShowAll;
	static bool mWriteOrDebug;	// Ϊtrue��ʾ���ڴ���Ϣд�빲���ڴ�,false��ʾ����ӡ������̨
	static ThreadLock mInfoLock;
	static CustomThread* mThread;
	static txShareMemoryServer* mShareMemoryServer;
	static txMap<std::string, int> mMemoryTypeIndex;
	static MemoryType mMemoryList[MAX_COUNT];
	static int mMemoryCount;
};
#endif

#endif