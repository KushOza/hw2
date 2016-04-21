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
	//thread->pid = nextpid++;
	thread->pid = clone((void *) start_routine, arg, stack);
	return 0;
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

int pthread_mutex_destroy(pthread_mutex_t *mutex){
	return mutex_destroy(mutex->id);
};
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr){
	return mutex_init();
};
int pthread_mutex_lock(pthread_mutex_t *mutex){
	return mutex_lock(mutex->id);
};
int pthread_mutex_unlock(pthread_mutex_t *mutex){
	return mutex_unlock(mutex->id);
};
