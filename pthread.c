#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

// Implement your pthreads library here.

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	return clone((void *) start_routine, arg, (void *) thread);
}

int pthread_join(pthread_t thread, void **retval)
{
	return join(thread.pid, (void **)&thread, (void **)retval);
}
int pthread_exit(void *retval)
{
	texit(retval);
	return 0;
}
