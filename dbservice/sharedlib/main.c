#include <stdio.h>
#include <stdlib.h>

#include "libenvmap.h"

#include "Windows.h"

int main(int argc, char **argv) {

	graal_isolate_t *isolate = NULL ;
	graal_isolatethread_t *thread = NULL ;

	if ( graal_create_isolate(NULL, &isolate, &thread) != 0 ) {
		fprintf( stderr, "initialization error\n" ) ;
		return 1 ;
	}

	int result = add( thread, 2, 2 ) ;

	printf( "%d\n", result ) ;

	Sleep(1000);

	result = multadd( thread, 2, 3 ) ;

	printf( "%d\n", result );

	graal_tear_down_isolate( thread ) ;

}

