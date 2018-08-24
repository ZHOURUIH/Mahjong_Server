#ifndef _SQL_ACCOUNT_H_
#define _SQL_ACCOUNT_H_

#include "SQLTableBase.h"

class AccountTable : public TableBase
{
public:
	std::string mAccount;
	std::string mPassword;
	int mGUID;
	bool mIsRobot;
};

class SQLAccount : public SQLTableBase
{
public:
	SQLAccount(const char* name)
		:SQLTableBase(name){}
	virtual ~SQLAccount(){ destroy(); }
	void destroy() {}
	virtual void init(MYSQL* mysql)
	{
		SQLTableBase::init(mysql);
	}
	bool queryData(AccountTable* tableData);
	int getMaxGUID();
	bool isAccountExist(const std::string& account);
	bool registerAccount(AccountTable* tableData);
	bool getAllRobotAccount(txMap<CHAR_GUID, AccountTable*>& robotAccountList);
protected:
	static const char* COL_ACCOUNT;
	static const char* COL_PASSWORD;
	static const char* COL_IS_ROBOT;
	static const char* COL_GUID;
};

#endif
