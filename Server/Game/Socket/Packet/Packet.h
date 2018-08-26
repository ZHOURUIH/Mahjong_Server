﻿#ifndef _PACKET_H_
#define _PACKET_H_

#include "txSerializedData.h"

#include "PacketDefine.h"
#ifdef _SERVER
#include "ServerBase.h"
#endif

class Packet : public txSerializedData
#ifdef _SERVER
	, public ServerBase
#endif
{
public:
	Packet(PACKET_TYPE type)
		:
		mType(type),
		mClient(INVALID_ID)
	{}
	virtual void init()
	{
		fillParams();
		zeroParams();
	}
	PACKET_TYPE getPacketType() { return mType; }
	virtual void execute(){}
	virtual std::string debugInfo()
	{
		PACKET_DEBUG(PACKET_EMPTY);
	}
public:
	PACKET_TYPE mType;
	CLIENT_GUID mClient;	// 客户端ID
};

#endif
