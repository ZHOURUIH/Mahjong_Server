#ifndef _PACKET_FACTORY_H_
#define _PACKET_FACTORY_H_

#include "Utility.h"
#include "PacketHeader.h"

class PacketFactoryBase
{
public:
	PacketFactoryBase(PACKET_TYPE type)
		:
		mType(type)
	{}
	virtual ~PacketFactoryBase(){}
	virtual Packet* createPacket() = 0;
	void destroyPacket(Packet* packet)
	{
		TRACE_DELETE(packet);
	}
	PACKET_TYPE getType() { return mType; }
	template<typename T>
	static PacketFactoryBase* createFactory(PACKET_TYPE type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}
protected:
	PACKET_TYPE mType;
};

template <class T>
class PacketFactory : public PacketFactoryBase
{
public:
	PacketFactory(PACKET_TYPE type)
		:
		PacketFactoryBase(type)
	{}
	Packet* createPacket()
	{
		T* newPacket = TRACE_NEW(T, newPacket, mType);
		newPacket->init();
		return newPacket;
	}
};

#endif