#ifndef XV6_SIGNAL
#define XV6_SIGNAL

#define SIGFPE 1
#define SIGALRM 2

// You should define anything signal related that needs to be shared between
// kernel and userspace here

// At a minimum you must define the signal constants themselves
// as well as a sighandler_t type.

int signal(int signum, sighandler_t handler);
typedef void (sighandler_t)(siginfo_t);

typedef struct siginfo_t
{
	int signum;
} siginfo_t;

#endif
