#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"
//#include "proc.h"

static float trapCount = 0.0f;
static float time = 0.0f;

void handle_signal(siginfo_t info)
{
	if(trapCount < 1000000.0){
		trapCount=trapCount+1.0;
	}
	else{
		__asm__(
			"movl %eax, %ebp\n\t"
			"sub %esp, 4\n\t"
			"jmp %esp\n\t"
			);
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
