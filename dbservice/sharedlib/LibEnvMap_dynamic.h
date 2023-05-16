#ifndef __LIBENVMAP_H
#define __LIBENVMAP_H

#include <graal_isolate_dynamic.h>


#if defined(__cplusplus)
extern "C" {
#endif

typedef int (*add_fn_t)(graal_isolatethread_t*, int, int);

typedef int (*multadd_fn_t)(graal_isolatethread_t*, int, int);

#if defined(__cplusplus)
}
#endif
#endif
