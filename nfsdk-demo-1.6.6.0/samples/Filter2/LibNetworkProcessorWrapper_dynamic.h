#ifndef __LIBNETWORKPROCESSORWRAPPER_H
#define __LIBNETWORKPROCESSORWRAPPER_H

#include "graal_isolate_dynamic.h"


#if defined(__cplusplus)
extern "C" {
#endif

typedef int (*parse_dns_payload_fn_t)(graal_isolatethread_t*, char*);

#if defined(__cplusplus)
}
#endif
#endif
