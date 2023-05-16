#ifndef __LIBENVMAP_H
#define __LIBENVMAP_H

#include <graal_isolate.h>


#if defined(__cplusplus)
extern "C" {
#endif

int add(graal_isolatethread_t*, int, int);

int multadd(graal_isolatethread_t*, int, int);

#if defined(__cplusplus)
}
#endif
#endif
