/**
*	The sample filters all TCP connections and UDP datagrams in pass-through mode,
*	and prints the information about all called events to standard console output.
**/

#include "defines.h"
#include "stdafx.h" // winsock2, ws2tcpip, stdio headers

#ifdef _ENABLE_LOG
#include "logger.h"
#endif
//#include <crtdbg.h>

#include "EventHandler.h"

#include "nfapi.h"

using namespace nfapi;

// Change this string after renaming and registering the driver under different name
#define NFDRIVER_NAME "netfilter2"

int main(int argc, char* argv[])
{
	// Stop console Window opening, it needs to be ran as Admin
    // Ref: https://stackoverflow.com/questions/2139637/hide-console-of-windows-application/12413270#12413270

#ifdef _NOCONSOLE
	HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, 0);
#endif

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

	// Initialize the library and start filtering thread
	if (nf_init(NFDRIVER_NAME, &eh) != NF_STATUS_SUCCESS)
	{
#ifdef _ENABLE_LOG
		logMessage("Failed to connect to driver");
#endif
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
	int c = getchar();

#ifdef _ENABLE_LOG
	// Close the log file
	closeLogFile();
#endif

	// Free the library
	nf_free();

	::WSACleanup();

	return 0;
}