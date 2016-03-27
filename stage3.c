#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"
//#include "proc.h"

static float trapCount = 0.0f;
static float time = 0.0f;

void handle_signal(siginfo_t info)
{
	printf(1, "Caught signal %d...\n", info.signum);
	if (info.signum == SIGFPE)
		printf(1, "TEST PASSED\n");
	else
		printf(1, "TEST FAILED: wrong signal sent.\n");

	if(trapCount < 10.0f){
		trapCount=trapCount+1.0;
		printf(1,"%d",trapCount);
	}
	else{
		__asm__(
			"movl %eax, %esp\n\t"
			"add 4, %esp\n\t"
			"jmp %esp\n\t"
			);
		printf(1,"else");
	}
}

int main(int argc, char *argv[])
{
	signal(SIGFPE, handle_signal);

	int x = 1;
	int y = 0;
	x = x/y;

	time = uptime();
	float avg = trapCount/time;

	printf(1, "Traps Performed: %d\n", trapCount);
	printf(1, "Total Elapsed Time: %d\n", time);
	printf(1, "Average Time Per Trap: %d\n", avg);

	exit();
}
