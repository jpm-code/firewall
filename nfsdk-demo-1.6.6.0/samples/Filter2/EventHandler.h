#pragma once

#include "stdafx.h"

#include "utils.h"

#ifdef _ENABLE_LOG
#include "logger.h"
#endif

#include "dns.h"

#include <array>

#include "nfapi.h"

using namespace nfapi;


#include <string>
#include <sstream>
//#include <vector>

//
//	API events handler
//
class EventHandler : public NF_EventHandler
{

	virtual void threadStart();
	virtual void threadEnd();

	//
	// TCP events
	//
	virtual void tcpConnectRequest(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo);

	virtual void tcpConnected(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo);

	virtual void tcpClosed(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo);

	virtual void tcpReceive(ENDPOINT_ID id, const char* buf, int len);

	virtual void tcpSend(ENDPOINT_ID id, const char* buf, int len);

	virtual void tcpCanReceive(ENDPOINT_ID id);

	virtual void tcpCanSend(ENDPOINT_ID id);

	//
	// UDP events
	//
	virtual void udpCreated(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo);

	virtual void udpConnectRequest(ENDPOINT_ID id, PNF_UDP_CONN_REQUEST pConnReq);

	virtual void udpClosed(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo);

	virtual void udpReceive(ENDPOINT_ID id, const unsigned char* remoteAddress, const char* buf, int len, PNF_UDP_OPTIONS options);
	
	virtual void udpSend(ENDPOINT_ID id, const unsigned char* remoteAddress, const char* buf, int len, PNF_UDP_OPTIONS options);

	virtual void udpCanReceive(ENDPOINT_ID id);

	virtual void udpCanSend(ENDPOINT_ID id);

};

