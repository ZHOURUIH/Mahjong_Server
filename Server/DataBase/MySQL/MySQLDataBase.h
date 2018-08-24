#ifndef _MYSQL_DATA_BASE_H_
#define _MYSQL_DATA_BASE_H_

#include "FrameComponent.h"

class SQLAccount;
class SQLCharacterData;
class AccountTable;
class CharacterDataTable;
class MySQLDataBase : public FrameComponent
{
public:
	MySQLDataBase(const std::string& name);
	virtual ~MySQLDataBase(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime){}
	void destroy();
	// 查询登陆,根据账号密码获得GUID,需要指定是否为机器人登录
	bool queryLogin(AccountTable* data);
	// 查询角色信息
	bool queryCharacterData(CHAR_GUID guid, CharacterDataTable* data);
	// 账号是否存在
	bool isAccountExist(const std::string& account);
	// 名字是否存在
	bool isNameExist(const std::string& name);
	// 注册账号,0表示成功,-1表示账号已存在,-2表示名字已存在, -3代表GUID计算错误, -4表示账号插入错误, -5表示角色数据插入错误,-6表示连接数据库失败
	int registerAccount(AccountTable* accountData, CharacterDataTable* data);
	bool getAllRobotAccount(txMap<CHAR_GUID, AccountTable*>& robotAccountList);
protected:
	bool connectDataBase(const std::string& dataBase);
	int getMaxGUID();
	
protected:
	MYSQL* mMySQL;
	std::string mConnectedDataBase; // 已经连接的数据库
	std::string mUser;
	std::string mPassword;
	std::string mHost;
	SQLAccount* mSQLAccount;
	SQLCharacterData* mSQLCharacterData;
	static const char* DATABASE_MAHJONG_USERDATA;
	static const char* TABLE_ACCOUNT;
	static const char* TABLE_CHARACTER_DATA;
	static const char* COL_ACCOUNT;
	static const char* COL_PASSWORD;
	static const char* COL_IS_ROBOT;
	static const char* COL_GUID;
	static const char* COL_NAME;
	static const char* COL_MONEY;
	static const char* COL_HEAD;
	int mPort;
};

#endif
