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
		std::map<PACKET_TYPE, PacketFactoryBase*>::iterator iter = mFactoryList.begin();
		std::map<PACKET_TYPE, PacketFactoryBase*>::iterator iterEnd = mFactoryList.end();
		for (; iter != iterEnd; ++iter)
		{
			TRACE_DELETE(iter->second);
		}
		mFactoryList.clear();
	}
	static int getPacketSize(const PACKET_TYPE& type)
	{
		std::map<PACKET_TYPE, int>::iterator iter = mPacketSizeMap.find(type);
		if (iter != mPacketSizeMap.end())
		{
			return iter->second;
		}
		return -1;
	}
	void addPacketSize(const PACKET_TYPE& type)
	{
		PacketFactoryBase* factory = getFactory(type);
		if (factory == NULL)
		{
			mPacketSizeMap.insert(std::make_pair(type, -1));
		}
		else
		{
			Packet* packet = factory->createPacket();
			mPacketSizeMap.insert(std::make_pair(type, packet->getSize()));
			factory->destroyPacket(packet);
		}
	}
	PacketFactoryBase* getFactory(const PACKET_TYPE& type)
	{
		std::map<PACKET_TYPE, PacketFactoryBase*>::iterator iter = mFactoryList.find(type);
		if (iter != mFactoryList.end())
		{
			return iter->second;
		}
		return NULL;
	}
	std::map<PACKET_TYPE, PacketFactoryBase*>& getFactoryList() { return mFactoryList; }
protected:
	template<typename O>
	PacketFactoryBase* addFactory(const PACKET_TYPE& type)
	{
		PacketFactoryBase* factory = PacketFactoryBase::createFactory<O>(type);
		mFactoryList.insert(std::make_pair(factory->getType(), factory));
		return factory;
	}
protected:
	static std::map<PACKET_TYPE, int> mPacketSizeMap;
	std::map<PACKET_TYPE, PacketFactoryBase*> mFactoryList;
};

#endif
