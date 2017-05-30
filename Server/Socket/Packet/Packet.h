#ifndef _PACKET_H_
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
	Packet(const PACKET_TYPE& type)
		:
		mType(type),
		mClient(~0)
	{}
	const PACKET_TYPE& getPacketType() { return mType; }
	virtual void execute() = 0;
	virtual std::string debugInfo() = 0;
public:
	PACKET_TYPE mType;
	CLIENT_GUID mClient;	// 客户端ID
};

#endif
