#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "signal.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}

int sys_register_signal_handler(void)
{
	int signum = 0;
	int handler = 0;
	if (argint(0, &signum) < 0 || argint(1, &handler) < 0)
	{
		cprintf("Something bad happened\n");
		return -1;
	}
	//cprintf("The value of signum is %d\n", signum);
	if (signum == SIGFPE)
	{
		//cprintf("Got dat sigfpe boi\n");
		proc->signalhandlers[0] = handler;
	}
	if (signum == SIGALRM)
	{
		//cprintf("Got dat sigalrm boi\n");
		proc->signalhandlers[1] = handler;
	}
	return signum;
}

int sys_sigalrm_handler(void)
{
	int alarmTime = 0;
	if (argint(0, &alarmTime) < 0)
	    return -1;
	alarmTime = alarmTime*1000;
	proc->alarmCounter = alarmTime;
	return proc->alarmCounter;
}
