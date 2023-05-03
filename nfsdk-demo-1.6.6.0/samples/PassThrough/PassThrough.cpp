/**
*	The sample filters all TCP connections and UDP datagrams in pass-through mode,
*	and prints the information about all called events to standard console output.
**/

#include "stdafx.h"
#include <crtdbg.h>
#include "nfapi.h"
#include <array>
#include <regex>
#include "dns.h"
#include <iostream>

#include <fstream>
#include <iostream>
#include <sstream>

#include <iomanip>
#include <chrono>


using namespace nfapi;

// Change this string after renaming and registering the driver under different name
#define NFDRIVER_NAME "netfilter2"

// Forward declarations
void printConnInfo(bool connected, ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo);
void printAddrInfo(bool created, ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo);
std::string get_logger_filename();
void logMessage(std::string logMsg);
std::string get_domain_logger_filename();
void logDomain(std::string domainMsg);

std::string new_filename = get_logger_filename();
std::ofstream outfile(new_filename);

std::string domainLogFilename = get_domain_logger_filename();
std::ofstream domainLogFile(domainLogFilename);

//
//	API events handler
//
class EventHandler : public NF_EventHandler
{
	virtual void threadStart()
	{
		//printf("threadStart\n");
		//fflush(stdout);

		// Initialize thread specific stuff
	}

	virtual void threadEnd()
	{
		//printf("threadEnd\n");

		// Uninitialize thread specific stuff
	}
	
	//
	// TCP events
	//

	virtual void tcpConnectRequest(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
	{
		//printf("tcpConnectRequest id=%I64u\n", id);
	}

	virtual void tcpConnected(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
	{
	    //printConnInfo(true, id, pConnInfo);
		//fflush(stdout);
	}

	virtual void tcpClosed(ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
	{
		//printConnInfo(false, id, pConnInfo);
		//fflush(stdout);
	}

	virtual void tcpReceive(ENDPOINT_ID id, const char * buf, int len)
	{	
		//printf("tcpReceive id=%I64u len=%d\n", id, len);
		//fflush(stdout);

		// Send the packet to application
		nf_tcpPostReceive(id, buf, len);
	}

	virtual void tcpSend(ENDPOINT_ID id, const char * buf, int len)
	{
		//printf("tcpSend id=%I64u len=%d\n", id, len);
		//fflush(stdout);

		// Send the packet to server
		nf_tcpPostSend(id, buf, len);
	}

	virtual void tcpCanReceive(ENDPOINT_ID id)
	{
		//printf("tcpCanReceive id=%I64d\n", id);
		//fflush(stdout);
	}

	virtual void tcpCanSend(ENDPOINT_ID id)
	{
		//printf("tcpCanSend id=%I64d\n", id);
		//fflush(stdout);
	}
	
	//
	// UDP events
	//

	virtual void udpCreated(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
	{
		printAddrInfo(true, id, pConnInfo);
		//fflush(stdout);
	}

	virtual void udpConnectRequest(ENDPOINT_ID id, PNF_UDP_CONN_REQUEST pConnReq)
	{
		char* buffer;
		int size = std::snprintf(nullptr, 0, "udpConnectRequest id=%I64u\n",
			id);
		buffer = (char*)std::malloc(size + 1);
		std::sprintf(buffer, "udpConnectRequest id=%I64u\n",
			id);

		logMessage(buffer);
		std::free(buffer);

		//printf("udpConnectRequest id=%I64u\n", id);
	}

	virtual void udpClosed(ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
	{
		printAddrInfo(false, id, pConnInfo);
		//fflush(stdout);
	}

	virtual void udpReceive(ENDPOINT_ID id, const unsigned char * remoteAddress, const char * buf, int len, PNF_UDP_OPTIONS options)
	{	
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

		//printf(">> udpReceive id=%I64u len=%d remoteAddress=%s\n", id, len, remoteAddr.data());
		//fflush(stdout);

		// Send the packet to application
		nf_udpPostReceive(id,
			remoteAddress,
			buf,
			len,
			options
		);

	}

	virtual void udpSend(ENDPOINT_ID id, const unsigned char * remoteAddress, const char * buf, int len, PNF_UDP_OPTIONS options)
	{
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

		//printf("<< udpSend id=%I64u len=%d remoteAddress=%s flags=%x optionsLen=%d\n",
		//	id, len, remoteAddr, options->flags, options->optionsLength
		//);
		//fflush(stdout);
		
		//
		// Check for standard qurey and one domain, then parse
		//
		if (buf[2] == 0x01 && buf[3] == 0x00 && buf[4] == 0x00 && buf[5] == 0x01) {

			std::string domain = parse_dns_query_domain(buf, len);

			logDomain( "Domain: " + domain );

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
		nf_udpPostSend(id,
			remoteAddress,
			buf,
			len,
			options
		);

	}

	virtual void udpCanReceive(ENDPOINT_ID id)
	{
		//printf("udpCanReceive id=%I64d\n", id);
		//fflush(stdout);
	}

	virtual void udpCanSend(ENDPOINT_ID id)
	{
		//printf("udpCanSend id=%I64d\n", id);
		//fflush(stdout);
	}
};

int main(int argc, char* argv[])
{
	// Stop console Window opening, it needs to be ran as Admin
    // Ref: https://stackoverflow.com/questions/2139637/hide-console-of-windows-application/12413270#12413270
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, 0);

	EventHandler eh;
	NF_RULE rule;
	WSADATA wsaData;

	// This call is required for WSAAddressToString
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	nf_adjustProcessPriviledges();

	//printf("Press enter to stop...\n\n");

	// Initialize the library and start filtering thread
	if (nf_init(NFDRIVER_NAME, &eh) != NF_STATUS_SUCCESS)
	{
		logMessage("Failed to connect to driver");
		return -1;
	}

	
	// Filter all TCP/UDP traffic
	//memset(&rule, 0, sizeof(rule));
	//rule.filteringFlag = NF_FILTER; // | NF_INDICATE_CONNECT_REQUESTS;
	//nf_addRule(&rule, TRUE);

	// Filter outgoing IPv4 UDP packets, directed to port 53 (DNS)
	memset(&rule, 0, sizeof(rule));
	rule.ip_family = AF_INET;
	rule.protocol = IPPROTO_UDP;
	rule.direction = NF_D_BOTH; //NF_D_OUT;
	rule.remotePort = htons(53);
	rule.filteringFlag = NF_FILTER | NF_INDICATE_CONNECT_REQUESTS;
	nf_addRule(&rule, TRUE);


	// Wait for any key
	getchar();

	// Close log file
	outfile.close();

	// Free the library
	nf_free();

	::WSACleanup();

	return 0;
}

/**
* Print the connection information
**/
void printConnInfo(bool connected, ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo)
{
	char localAddr[MAX_PATH] = "";
	char remoteAddr[MAX_PATH] = "";
	DWORD dwLen;
	sockaddr * pAddr;
	char processName[MAX_PATH] = "";
	
	pAddr = (sockaddr*)pConnInfo->localAddress;
	dwLen = sizeof(localAddr);

	WSAAddressToString((LPSOCKADDR)pAddr, 
				(pAddr->sa_family == AF_INET6)? sizeof(sockaddr_in6) : sizeof(sockaddr_in), 
				NULL, 
				localAddr, 
				&dwLen);

	pAddr = (sockaddr*)pConnInfo->remoteAddress;
	dwLen = sizeof(remoteAddr);

	WSAAddressToString((LPSOCKADDR)pAddr, 
				(pAddr->sa_family == AF_INET6)? sizeof(sockaddr_in6) : sizeof(sockaddr_in), 
				NULL, 
				remoteAddr, 
				&dwLen);
	
	if (connected)
	{
		if (!nf_getProcessName(pConnInfo->processId, processName, sizeof(processName)/sizeof(processName[0])))
		{
			processName[0] = '\0';
		}

		printf("tcpConnected id=%I64u flag=%d pid=%d direction=%s local=%s remote=%s (conn.table size %d)\n\tProcess: %s\n",
			id,
			pConnInfo->filteringFlag,
			pConnInfo->processId, 
			(pConnInfo->direction == NF_D_IN)? "in" : ((pConnInfo->direction == NF_D_OUT)? "out" : "none"),
			localAddr, 
			remoteAddr,
			nf_getConnCount(),
			processName);
	} else
	{
		printf("tcpClosed id=%I64u flag=%d pid=%d direction=%s local=%s remote=%s (conn.table size %d)\n",
			id,
			pConnInfo->filteringFlag,
			pConnInfo->processId, 
			(pConnInfo->direction == NF_D_IN)? "in" : ((pConnInfo->direction == NF_D_OUT)? "out" : "none"),
			localAddr, 
			remoteAddr,
			nf_getConnCount());
	}

}

/**
* Print the address information
**/
void printAddrInfo(bool created, ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo)
{
	char localAddr[MAX_PATH] = "";
	sockaddr * pAddr;
	DWORD dwLen;
	char processName[MAX_PATH] = "";
	
	pAddr = (sockaddr*)pConnInfo->localAddress;
	dwLen = sizeof(localAddr);

	WSAAddressToString((LPSOCKADDR)pAddr, 
				(pAddr->sa_family == AF_INET6)? sizeof(sockaddr_in6) : sizeof(sockaddr_in), 
				NULL, 
				localAddr, 
				&dwLen);
		
	if (created)
	{
		if (!nf_getProcessName(pConnInfo->processId, processName, sizeof(processName)/sizeof(processName[0])))
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


	} else
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

/**
* Get the logger filename
**/
std::string get_logger_filename() {

	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
	std::string base_filename = "log-" + ss.str() + ".txt";

	std::string new_filename = base_filename;
	int index = 1;

	while (std::ifstream(new_filename)) {
		std::stringstream ss;
		ss << base_filename << "_" << index;
		new_filename = ss.str();
		index++;
	}

	return new_filename;
}

void logMessage(std::string logMsg)
{
	if (outfile.is_open()) {
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << " [" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] ";
		outfile << ss.str() << logMsg << std::endl;
		
	}
}

std::string get_domain_logger_filename() 
{

		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
		std::string base_filename = "domains-" + ss.str() + ".txt";

		std::string new_filename = base_filename;
		int index = 1;

		while (std::ifstream(new_filename)) {
			std::stringstream ss;
			ss << base_filename << "_" << index;
			new_filename = ss.str();
			index++;
		}

		return new_filename;
}

void logDomain(std::string domainMsg)
{
	if (domainLogFile.is_open()) {
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << " [" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] ";
		domainLogFile << ss.str() << domainMsg << std::endl;
	}
}