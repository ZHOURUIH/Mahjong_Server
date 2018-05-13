#ifndef _CHARACTER_DATA_H_
#define _CHARACTER_DATA_H_

#include "ServerDefine.h"
#include "GameDefine.h"

class CharacterData
{
public:
	CharacterData()
	{
		mGUID = INVALID_ID;
		mHead = 0;
		mMoney = 0;
		mBanker = false;
		mPosition = -1;
		mRoomID = INVALID_INT_ID;
		mReady = false;
	}
	virtual ~CharacterData(){}
public:
	std::string mName;						// 鍚嶅瓧
	CHAR_GUID mGUID;						// 鍞竴ID
	int mHead;								// 澶村儚
	int mMoney;								// 閲戝竵鏁?
	bool mBanker;							// 鏄惁涓哄簞瀹?
	int mPosition;							// 鐜╁鐨勪綅缃?鎴块棿涓殑椤哄簭,瀹㈡埛绔腑闇€瑕佷緷鎹椤哄簭,閲嶆柊鎺掑垪鑷繁甯冨眬鐨勪綅缃?
	int mRoomID;							// 鐜╁褰撳墠鎵€鍦ㄧ殑鎴块棿ID
	bool mReady;							// 鐜╁鏄惁宸插噯澶?
	txVector<MAHJONG> mHandIn;				// 鎵嬮噷鐨勭墝
	txVector<MAHJONG> mDropList;			// 鎵撳嚭鐨勭墝
	txVector<PengGangInfo*> mPengGangList;	// 纰版垨鑰呮潬鐨勭墝
	txVector<MAHJONG> mHuaList;				// 鎽稿埌鐨勮姳鐗?
};

#endif