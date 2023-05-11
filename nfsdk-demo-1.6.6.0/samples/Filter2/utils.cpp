#include "stdafx.h"
#include "utils.h"

/**
* Print the connection information
**/
void printConnInfo(bool connected, ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
{
	char localAddr[MAX_PATH] = "";
	char remoteAddr[MAX_PATH] = "";
	DWORD dwLen;
	sockaddr* pAddr;
	char processName[MAX_PATH] = "";

	pAddr = (sockaddr*)pConnInfo->localAddress;
	dwLen = sizeof(localAddr);

	WSAAddressToString((LPSOCKADDR)pAddr,
		(pAddr->sa_family == AF_INET6) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in),
		NULL,
		localAddr,
		&dwLen);

	pAddr = (sockaddr*)pConnInfo->remoteAddress;
	dwLen = sizeof(remoteAddr);

	WSAAddressToString((LPSOCKADDR)pAddr,
		(pAddr->sa_family == AF_INET6) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in),
		NULL,
		remoteAddr,
		&dwLen);

	if (connected)
	{
		if (!nf_getProcessName(pConnInfo->processId, processName, sizeof(processName) / sizeof(processName[0])))
		{
			processName[0] = '\0';
		}

		printf("tcpConnected id=%I64u flag=%d pid=%d direction=%s local=%s remote=%s (conn.table size %d)\n\tProcess: %s\n",
			id,
			pConnInfo->filteringFlag,
			pConnInfo->processId,
			(pConnInfo->direction == NF_D_IN) ? "in" : ((pConnInfo->direction == NF_D_OUT) ? "out" : "none"),
			localAddr,
			remoteAddr,
			nf_getConnCount(),
			processName);
	}
	else
	{
		printf("tcpClosed id=%I64u flag=%d pid=%d direction=%s local=%s remote=%s (conn.table size %d)\n",
			id,
			pConnInfo->filteringFlag,
			pConnInfo->processId,
			(pConnInfo->direction == NF_D_IN) ? "in" : ((pConnInfo->direction == NF_D_OUT) ? "out" : "none"),
			localAddr,
			remoteAddr,
			nf_getConnCount());
	}

}

/*
*  Print the address information
*/
void printAddrInfo(bool created, ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
{
	char localAddr[MAX_PATH] = "" ;
	sockaddr* pAddr ;
	DWORD dwLen ;
	char processName[MAX_PATH] = "" ;

	pAddr = (sockaddr*)pConnInfo -> localAddress ;
	dwLen = sizeof(localAddr);

	WSAAddressToString( (LPSOCKADDR)pAddr,
		                (pAddr->sa_family == AF_INET6) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in),
		                NULL,
		                localAddr,
		                &dwLen
	                  );

	if (created)
	{
		if (!nf_getProcessName(pConnInfo->processId, processName, sizeof(processName) / sizeof(processName[0])))
		{
			processName[0] = '\0';
		}

		char* buffer;
		int size = std::snprintf(nullptr, 0, "udpCreated id=%I64u pid=%d local=%s\n\tProcess: %s\n",
			id,
			pConnInfo->processId,
			localAddr,
			processName);
		buffer = (char*)std::malloc(size + 1);
		std::sprintf(buffer, "udpCreated id=%I64u pid=%d local=%s\n\tProcess: %s\n",
			id,
			pConnInfo->processId,
			localAddr,
			processName);

		logMessage(buffer);
		std::free(buffer);

		/*printf("udpCreated id=%I64u pid=%d local=%s\n\tProcess: %s\n",
			id,
			pConnInfo->processId,
			localAddr,
			processName); */


	}
	else
	{
		char* buffer;
		int size = std::snprintf(nullptr, 0, "udpClosed id=%I64u pid=%d local=%s\n",
			id,
			pConnInfo->processId,
			localAddr);
		buffer = (char*)std::malloc(size + 1);
		std::sprintf(buffer, "udpClosed id=%I64u pid=%d local=%s\n",
			id,
			pConnInfo->processId,
			localAddr);

		logMessage(buffer);
		std::free(buffer);

		/*printf("udpClosed id=%I64u pid=%d local=%s\n",
			id,
			pConnInfo->processId,
			localAddr);
			*/
	}

}

