#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

// Implement your pthreads library here.

int nextpid = 0;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	void* stack=malloc(4096);
	thread->pid = nextpid++;
	return clone((void *) start_routine, arg, stack);
}

int pthread_join(pthread_t thread, void **retval)
{
	void* stack=malloc(4096);
	return join(thread.pid, (void **)stack, (void **)retval);

}
int pthread_exit(void *retval)
{
	texit(retval);
	return 0;
}
