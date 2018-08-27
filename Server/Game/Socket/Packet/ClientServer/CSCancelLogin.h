#ifndef _CS_CANCEL_LOGIN_H_
#define _CS_CANCEL_LOGIN_H_

#include "Packet.h"

class CSCancelLogin : public Packet
{
public:
	CSCancelLogin(PACKET_TYPE type)
		:Packet(type) {}
	virtual void execute();
	virtual void fillParams() {}
public:
};

#endif