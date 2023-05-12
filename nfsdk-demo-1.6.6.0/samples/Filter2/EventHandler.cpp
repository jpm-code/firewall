#include "stdafx.h"
#include "Defines.h"
#include "EventHandler.h"

// Network processor dll includes - put in header?
#include <stdio.h>
#include <stdlib.h>

#include "LibNetworkProcessorWrapper.h"

void EventHandler::threadStart()
{
	//printf("threadStart\n");
	//fflush(stdout);

	// Initialize thread specific stuff
}

void EventHandler::threadEnd()
{
	//printf("threadEnd\n");

	// Uninitialize thread specific stuff
}

//
// TCP events
//

void EventHandler::tcpConnectRequest(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
{
	//printf("tcpConnectRequest id=%I64u\n", id);
}

void EventHandler::tcpConnected(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
{
	//printConnInfo(true, id, pConnInfo);
	//fflush(stdout);
}

void EventHandler::tcpClosed(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
{
	//printConnInfo(false, id, pConnInfo);
	//fflush(stdout);
}

void EventHandler::tcpReceive(ENDPOINT_ID id, const char* buf, int len)
{
	//printf("tcpReceive id=%I64u len=%d\n", id, len);
	//fflush(stdout);

	// Send the packet to application
	nf_tcpPostReceive(id, buf, len);
}

void EventHandler::tcpSend(ENDPOINT_ID id, const char* buf, int len)
{
	//printf("tcpSend id=%I64u len=%d\n", id, len);
	//fflush(stdout);

	// Send the packet to server
	nf_tcpPostSend(id, buf, len);
}

void EventHandler::tcpCanReceive(ENDPOINT_ID id)
{
	//printf("tcpCanReceive id=%I64d\n", id);
	//fflush(stdout);
}

void EventHandler::tcpCanSend(ENDPOINT_ID id)
{
	//printf("tcpCanSend id=%I64d\n", id);
	//fflush(stdout);
}

//
// UDP events
//

void EventHandler::udpCreated(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
{

#ifdef _UDPCREATED_LOG
	printAddrInfo(true, id, pConnInfo);
#endif

	//fflush(stdout);
}

void EventHandler::udpConnectRequest(ENDPOINT_ID id, PNF_UDP_CONN_REQUEST pConnReq)
{

#ifdef _UDPCONNECTREQUEST_LOG
	char* buffer;
	int size = std::snprintf(nullptr, 0, "udpConnectRequest id=%I64u\n",
		id);
	buffer = (char*)std::malloc(size + 1);
	std::sprintf(buffer, "udpConnectRequest id=%I64u\n",
		id);

	logMessage(buffer);
	std::free(buffer);
#endif

	//printf("udpConnectRequest id=%I64u\n", id);
}

void EventHandler::udpClosed(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
{

#ifdef _UDPCLOSED_LOG
	printAddrInfo(false, id, pConnInfo);
#endif

	//fflush(stdout);
}

void EventHandler::udpReceive(ENDPOINT_ID id, const unsigned char* remoteAddress, const char* buf, int len, PNF_UDP_OPTIONS options)
{

#ifdef _UDPRECEIVE_LOG

	// Extract remote address to remoteAddr using winsock2's "WSAAddressToString" function
	std::array<char, MAX_PATH> remoteAddr;
	DWORD dwLen = sizeof(remoteAddr);
	DWORD* pDwLen = std::addressof(dwLen);
	WSAAddressToString((sockaddr*)remoteAddress,
		(((sockaddr*)remoteAddress)->sa_family == AF_INET6) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in),
		NULL,
		remoteAddr.data(), //output returned into remoteAddr[]
		pDwLen
	);

	// Print UDP packet details
	char* buffer;
	int size = std::snprintf(nullptr, 0, ">> udpReceive id=%I64u len=%d remoteAddress=%s\n",
		id, len, remoteAddr.data());
	buffer = (char*)std::malloc(size + 1);
	std::sprintf(buffer, ">> udpReceive id=%I64u len=%d remoteAddress=%s\n",
		id, len, remoteAddr.data());

	logMessage(buffer);
	std::free(buffer);
#endif

	//printf(">> udpReceive id=%I64u len=%d remoteAddress=%s\n", id, len, remoteAddr.data());
	//fflush(stdout);

	// Send the packet to application
	nf_udpPostReceive(id, remoteAddress, buf, len, options );

}

void EventHandler::udpSend(ENDPOINT_ID id, const unsigned char* remoteAddress, const char* buf, int len, PNF_UDP_OPTIONS options)
{

#ifdef _UDPSEND_LOG

	std::array<char, MAX_PATH> remoteAddr;
	DWORD dwLen = sizeof(remoteAddr);
	DWORD* pDwLen = std::addressof(dwLen);

	WSAAddressToString((sockaddr*)remoteAddress,
		(((sockaddr*)remoteAddress)->sa_family == AF_INET6) ? sizeof(sockaddr_in6) : sizeof(sockaddr_in),
		NULL,
		remoteAddr.data(),
		pDwLen
	);

	char* buffer;
	int size = std::snprintf(nullptr, 0, "<< udpSend id=%I64u len=%d remoteAddress=%s flags=%x optionsLen=%d\n",
		id, len, remoteAddr, options->flags, options->optionsLength);
	buffer = (char*)std::malloc(size + 1);
	std::sprintf(buffer, "<< udpSend id=%I64u len=%d remoteAddress=%s flags=%x optionsLen=%d\n",
		id, len, remoteAddr, options->flags, options->optionsLength);

	logMessage(buffer);
	std::free(buffer);

#endif

	//printf("<< udpSend id=%I64u len=%d remoteAddress=%s flags=%x optionsLen=%d\n",
	//	id, len, remoteAddr, options->flags, options->optionsLength
	//);
	//fflush(stdout);

	//
	// Check for standard qurey and one domain, then parse
	//
	int decision;
	if ( buf[2] == 0x01 && buf[3] == 0x00 && buf[4] == 0x00 && buf[5] == 0x01 ) {

		std::string domain = parse_dns_query_domain( buf, len ) ;

		//logDomain( "Domain: " + domain ) ;


		graal_isolate_t* isolate = NULL;
		graal_isolatethread_t* thread = NULL;

		if ( graal_create_isolate( NULL, &isolate, &thread ) != 0 ) {
			fprintf( stderr, "initialization error\n" ) ;
			exit( -1 ) ;
		}

		//char* str = "google.com";

		decision = parse_dns_payload( thread, (char*)domain.c_str() ) ;



		graal_tear_down_isolate( thread ) ;


		// Parse string by semi-colon
		/*std::string str = "ERRCODE3;mydomain.com";
		std::istringstream iss(str);
		std::string token;

		while ( std::getline(iss, token, ';') ) {
			// Do something with token
		}
		*/

		/*
		#include <string>
		#include <sstream>
		#include <vector>

		std::string str = "ERRCODE3;mydomain.com";
		std::istringstream iss(str);
		std::string token;
		std::vector<std::string> tokens;

		while(std::getline(iss, token, ';')) {
			tokens.push_back(token);
		}
		*/
		
		//std::cout << "Domain: " << domain;
		/*
		std::regex str_regex("google", std::regex_constants::icase);
		if (std::regex_search(domain, str_regex)) {
			logDomain( " - Matched - Packet Dropped" );
			//std::cout << " - Matched - Packet Dropped" << std::endl;
			//std::cout << "Matched" << std::endl;
			// drop packet
			return;
		}
		else {
			//logDomain( "\n" );
			//std::cout << std::endl;
		}
		*/

	} //End if


	// Send the packet to server
	if ( decision == 0 ) {
		nf_udpPostSend(id, remoteAddress, buf, len, options);
	}
	else {
		// block packet
		std::cout << "Blocked packet" << std::endl ;
	}

}

void EventHandler::udpCanReceive(ENDPOINT_ID id)
{
	//printf("udpCanReceive id=%I64d\n", id);
	//fflush(stdout);
}

void EventHandler::udpCanSend(ENDPOINT_ID id)
{
	//printf("udpCanSend id=%I64d\n", id);
	//fflush(stdout);
}