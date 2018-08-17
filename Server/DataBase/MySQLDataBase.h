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
	virtual void init(const std::string& user, const std::string& pswd, const std::string& host, int port);
	virtual void update(float elapsedTime){}
	void destroy();
	// 查询登陆
	bool queryLogin(const std::string& account, const std::string& password, CHAR_GUID& guid);
	// 查询角色信息
	bool queryCharacterData(CHAR_GUID guid, std::string& name, int& money, short& head);
	// 账号是否存在
	bool isAccountExist(const std::string& account);
	// 名字是否存在
	bool isNameExist(const std::string& name);
	// 注册账号,0表示成功,-1表示账号已存在,-2表示名字已存在, -3代表GUID计算错误, -4表示账号插入错误, -5表示角色数据插入错误
	int registerAccount(const std::string& account, const std::string& password, const std::string& name, int money, int head);
protected:
	bool connectDataBase(const std::string& dataBase);
	int getMaxGUID();
	const std::string& getColumn(txMap<std::string, std::string>& rowData, const std::string& col, const std::string& table);
protected:
	MYSQL* mMySQL;
	std::string mConnectedDataBase; // 已经连接的数据库
	std::string mUser;
	std::string mPassword;
	std::string mHost;
	static const char* DATABASE_MAHJONG_USERDATA;
	static const char* TABLE_ACCOUNT;
	static const char* TABLE_CHARACTER_DATA;
	static const char* COL_ACCOUNT;
	static const char* COL_PASSWORD;
	static const char* COL_GUID;
	static const char* COL_NAME;
	static const char* COL_MONEY;
	static const char* COL_HEAD;
	int mPort;
};

#endif
