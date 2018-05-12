#ifndef _MEMORY_INFO_H_
#define _MEMORY_INFO_H_

#include "ServerDefine.h"

struct MemoryInfo
{
	MemoryInfo(int s, std::string f, int l, std::string t)
		:
		size(s),
		file(f),
		line(l),
		type(t)
	{}
	int size;			// �ڴ��С
	std::string file;	// �����ڴ���ļ�
	int line;			// �����ڴ�Ĵ����к�
	std::string type;	// �ڴ�Ķ�������
};

struct MemoryType
{
	MemoryType(const std::string& t = "")
		:
		type(t),
		count(0),
		size(0)
	{}
	MemoryType(const std::string& t, int c, int s)
		:
		type(t),
		count(c),
		size(s)
	{}
	std::string type;
	int count;
	int size;
};

#endif