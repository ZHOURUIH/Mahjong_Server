#ifndef _MYSQL_DATA_BASE_H_
#define _MYSQL_DATA_BASE_H_

#include "ServerDefine.h"

class MySQLDataBase
{
public:
	MySQLDataBase()
	:
		mMySQL(NULL),
		mPort(0)
	{}
	virtual ~MySQLDataBase(){ destroy(); }
	virtual void init(const std::string& user, const std::string& pswd, const std::string& host, const int& port);
	virtual void update(const float& elapsedTime){}
	void destroy();
	// 查询登陆
	bool queryLogin(const std::string& account, const std::string& password, std::string& playerName, CHAR_GUID& guid);
	// 账号是否存在
	bool isAccountExist(const std::string& account);
	// 名字是否存在
	bool isNameExist(const std::string& name);
	// 注册账号,0表示成功,-1表示账号已存在,-2表示名字已存在, -3表示其他错误
	int registerAccount(const std::string& account, const std::string& password, const std::string& name, const int& money, const int& head);
protected:
	bool connectDataBase(const std::string& dataBase);
	int getMaxGUID();
protected:
	MYSQL* mMySQL;
	std::string mConnectedDataBase; // 已经连接的数据库
	std::string mUser;
	std::string mPassword;
	std::string mHost;
	static std::string mDataBase;
	static std::string mAccountTable;
	static std::string mCharacterDataTable;
	static std::string mAccountColumnName;
	static std::string mPasswordColumnName;
	static std::string mGUIDColumnName;
	static std::string mNameColumnName;
	static std::string mModelColumnName;
	int mPort;
};

#endif
