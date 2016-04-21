#ifndef XV6_PTHREAD
#define XV6_PTHREAD
//#include <ucontext.h>
#include "spinlock.h"

// Define all functions and types for pthreads here.
// This can be included in both kernel and user code.

typedef struct pthread_mutex_t {
  int id;		//mutex id
  struct spinlock lock;		//actual spinlock
  int* locked;	//0 for not active, 1 for unlocked, 2 for locked
} pthread_mutex_t;

typedef struct pthread_t{
	int pid;
} pthread_t;

typedef struct pthread_attr_t{
	
} pthread_attr_t;

typedef struct pthread_mutexattr_t{
	
} pthread_mutexattr_t;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int pthread_join(pthread_t thread, void **retval);
int pthread_exit(void *retval);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

#endif