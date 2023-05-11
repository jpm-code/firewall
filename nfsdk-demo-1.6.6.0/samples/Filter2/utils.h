#pragma once
#include "stdafx.h"
#include "Defines.h"
#include "nfapi.h"
#ifdef _ENABLE_LOG
#include "logger.h"
#endif

using namespace nfapi;

void printConnInfo(bool connected, ENDPOINT_ID id, PNF_TCP_CONN_INFO pConnInfo);

void printAddrInfo(bool created, ENDPOINT_ID id, PNF_UDP_CONN_INFO pConnInfo);

