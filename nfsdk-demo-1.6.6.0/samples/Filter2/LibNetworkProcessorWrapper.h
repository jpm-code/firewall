#ifndef __LIBNETWORKPROCESSORWRAPPER_H
#define __LIBNETWORKPROCESSORWRAPPER_H

#include "graal_isolate.h"


#if defined(__cplusplus)
extern "C" {
#endif

int parse_dns_payload(graal_isolatethread_t*, char*);

#if defined(__cplusplus)
}
#endif
#endif
