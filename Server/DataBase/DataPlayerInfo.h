#ifndef _DATA_PLAYER_INFO_H_
#define _DATA_PLAYER_INFO_H_

#include "Data.h"

class DataPlayerInfo : public Data
{
public:
	DataPlayerInfo(const DATA_TYPE& type)
		:
		Data(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mGUID, "guid");
		pushParam(mModelID, "model id");
		pushArrayParam(mAccount, 16, "account");
		pushArrayParam(mPassword, 16, "password");
		pushArrayParam(mPlayerName, 16, "player name");
	}
public:
	int mGUID;
	int mModelID;
	char mAccount[16];
	char mPassword[16];
	char mPlayerName[16];
};

#endif
