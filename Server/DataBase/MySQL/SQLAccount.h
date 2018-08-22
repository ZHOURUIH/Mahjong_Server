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
	bool mIsLogin;
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
	bool queryData(AccountTable* tableData, const std::string& account, const std::string& password);
	int getMaxGUID();
	bool isAccountExist(const std::string& account);
	bool registerAccount(const std::string& account, const std::string& password, int guid, bool isRobot);
	bool getFirstNotLoginRobot(std::string& account, std::string& password);
	void notifyAccountLogin(CHAR_GUID guid, bool login);
protected:
	static const char* COL_ACCOUNT;
	static const char* COL_PASSWORD;
	static const char* COL_IS_ROBOT;
	static const char* COL_GUID;
	static const char* COL_IS_LOGIN;
};

#endif
