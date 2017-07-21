#ifndef _TX_DATA_MANAGER_H_
#define _TX_DATA_MANAGER_H_

#include "txDataElem.h"
#include "txThreadLock.h"

const int DATA_SLOTS = 1024;

class txDataManager
{
public:
	txDataManager();
	virtual ~txDataManager();
	virtual void init(const bool& showDebug = true) { mShowDebugInfo = showDebug; }
	void setShowDebugInfo(const bool& show) { mShowDebugInfo = show; }
	const bool& getShowDebugInfo() { return mShowDebugInfo; }
	int LoadData(const char* name, const bool& neednull = 0); //返回data的index -1=失败， neednull表示是否要在最后位添0
	int NewData(const char* name, const int& sizeInByte);
	//返回0=成功 1=失败
	bool DestroyData(const int& index);
	void DestroyAll();					//删除所有
	int GetDataIndex(char* name) const;		//通过数据名称获取数据的index， 返回-1表示未找到数据
	txDataElem* GetData(const int& index) const;		//通过index返回数据
	void dump() const;						//输出当前所有elems的情况,debug使用
	int GetFileSize(const int& index) const;			//根据文件的下标得到文件大小
	int GetFileSize(const char* filename, int* pStart, int* pLength, int* pFD) const; //根据文件名得到文件大小
	// 参数为media下相对路径或者绝对路径,或者相对于工作目录的相对路径
	bool isFileExist(const std::string& fileName) const;
protected:
	// 获取一个未使用的下标
	bool getOneUnusedIndex(int& index);
	// 将未使用列表中的一个下标放入已使用列表中,并且从未使用列表中删除该下标
	bool throwIndexToUsedList(const int& index);
	// 将已使用列表中的一个下标放入未使用列表中,并且从已使用列表中删除该下标
	bool throwIndexToUnusedList(const int& index);
	// 下标是否已经使用
	bool isIndexUsed(const int& index) const{ return mUsedIndexList.find(index) != mUsedIndexList.end(); }
	// 下标是否未被使用
	bool isIndexUnused(const int& index) const{ return mUnusedIndexList.find(index) != mUnusedIndexList.end(); }
	// 将全部下标都放入未使用列表中,并且清空已使用列表
	void throwAllToUnusedList();
protected:
	txDataElem elems[DATA_SLOTS];
	txMap<std::string, int> mDataNameList;	// first是文件名不带路径,second是文件下标
	txSet<short> mUsedIndexList;				// 已使用的下标列表
	txSet<short> mUnusedIndexList;			// 未使用的下标列表
	bool mShowDebugInfo;
	txThreadLock mDataLock;
};

#endif