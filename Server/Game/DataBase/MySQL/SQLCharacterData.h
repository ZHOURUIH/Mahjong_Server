﻿#ifndef _SQL_CHARACTER_DATA_H_
#define _SQL_CHARACTER_DATA_H_

#include "SQLTableBase.h"

class CharacterDataTable : public TableBase
{
public:
	int mGUID;
	std::string mName;
	int mMoney;
	int mHead;
};

class SQLCharacterData : public SQLTableBase
{
public:
	SQLCharacterData(const char*& tableName)
		:SQLTableBase(tableName){}
	virtual void init(MYSQL* mysql)
	{
		SQLTableBase::init(mysql);
	}
	bool queryCharacterData(CHAR_GUID guid, CharacterDataTable* tableData);
	bool isNameExist(const std::string& name);
	bool registeAccount(CharacterDataTable* data);
protected:
	static const char* COL_GUID;
	static const char* COL_NAME;
	static const char* COL_MONEY;
	static const char* COL_HEAD;
};

#endif