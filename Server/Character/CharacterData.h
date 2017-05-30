#ifndef _CHARACTER_DATA_H_
#define _CHARACTER_DATA_H_

#include "ServerDefine.h"

class CharacterData
{
public:
	CharacterData();
	virtual ~CharacterData(){}
public:
	std::string mName;		// 名稱
	int mModelID;			// 模型ID
	int mMapID;				// 玩家当前处于的地图或者想要匹配的地图ID
	int mRoomID;			// 玩家当前所在的房间ID
};

#endif