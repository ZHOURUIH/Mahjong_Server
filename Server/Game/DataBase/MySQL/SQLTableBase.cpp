#include "SQLTableBase.h"

void SQLTableBase::getResultData(txVector<txMap<std::string, std::string>>& resultData, MYSQL_RES* result)
{
	while (true)
	{
		MYSQL_ROW sql_row = mysql_fetch_row(result);
		if (sql_row == NULL)
		{
			break;
		}
		resultData.push_back(txMap<std::string, std::string>());
		// 重置列的下标
		mysql_field_seek(result, 0);
		int fieldCount = mysql_num_fields(result);
		for (int i = 0; i < fieldCount; ++i)
		{
			MYSQL_FIELD* fd = mysql_fetch_field(result);
			resultData[resultData.size() - 1].insert(fd->name, sql_row[i]);
		}
	}
}

const std::string& SQLTableBase::getColumn(txMap<std::string, std::string>& rowData, const std::string& col)
{
	if (rowData.find(col) == rowData.end())
	{
		LOG_ERROR("can not find %s", col.c_str());
		return EMPTY_STRING;
	}
	else
	{
		return rowData[col];
	}
}