#ifndef _PACKET_FACTORY_MANAGER_H_
#define _PACKET_FACTORY_MANAGER_H_

#include "Packet.h"
#include "PacketFactory.h"
#include "txUtility.h"

class PacketFactoryManager
{
public:
	virtual ~PacketFactoryManager(){destory();}
	virtual void init();
	virtual void destory()
	{
		auto iter = mFactoryList.begin();
		auto iterEnd = mFactoryList.end();
		FOR_STL(mFactoryList, ; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		END_FOR_STL(mFactoryList);
		mFactoryList.clear();
	}
	static int getPacketSize(PACKET_TYPE type)
	{
		auto iter = mPacketSizeMap.find(type);
		if (iter != mPacketSizeMap.end())
		{
			return iter->second;
		}
		return -1;
	}
	void addPacketSize(PACKET_TYPE type)
	{
		PacketFactoryBase* factory = getFactory(type);
		if (factory == NULL)
		{
			mPacketSizeMap.insert(type, -1);
		}
		else
		{
			Packet* packet = factory->createPacket();
			mPacketSizeMap.insert(type, packet->getSize());
			factory->destroyPacket(packet);
		}
	}
	PacketFactoryBase* getFactory(PACKET_TYPE type)
	{
		auto iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	txMap<PACKET_TYPE, PacketFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	PacketFactoryBase* addFactory(PACKET_TYPE type)
	{
		PacketFactoryBase* factory = PacketFactoryBase::createFactory<O>(type);
		mFactoryList.insert(factory->getType(), factory);
		return factory;
	}
protected:
	static txMap<PACKET_TYPE, int> mPacketSizeMap;
	txMap<PACKET_TYPE, PacketFactoryBase*> mFactoryList;
};

#endif
