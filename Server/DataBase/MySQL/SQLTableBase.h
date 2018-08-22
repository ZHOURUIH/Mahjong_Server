#ifndef _SQL_TABLE_BASE_H_
#define _SQL_TABLE_BASE_H_

#include "ServerBase.h"
#include "ServerDefine.h"

class TableBase
{};

class SQLTableBase : public ServerBase
{
public:
	SQLTableBase(const char* tableName)
	{
		mTableName = tableName;
	}
	virtual ~SQLTableBase(){ destroy(); }
	virtual void init(MYSQL* mysql)
	{
		mMySQL = mysql;
	}
	void destroy() {}
protected:
	static void getResultData(txVector<txMap<std::string, std::string>>& resultData, MYSQL_RES* result);
	static const std::string& getColumn(txMap<std::string, std::string>& rowData, const std::string& col);
protected:
	const char* mTableName;
	MYSQL* mMySQL;
};

#endif
