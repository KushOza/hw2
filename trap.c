#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "signal.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

struct proc* getProc(int index);

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
  
  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(proc->killed)
      exit();
    proc->tf = tf;
    syscall();
    if(proc->killed)
      exit();
    return;
  }
  //int i;
  switch(tf->trapno){
  case T_DIVIDE:
	  if (proc->signalhandlers[SIGFPE] > -1)	//confirm there is a sigfpe handler
	  {
		  siginfo_t sigfpeInfo;			//set new siginfo for SIGFPE
		  sigfpeInfo.signum = SIGFPE;
		  *((siginfo_t*)(proc->tf->esp - 4)) = sigfpeInfo;
		  proc->tf->esp = proc->tf->esp - 8;
		  proc->tf->eip = (uint) proc->signalhandlers[0];
		  break;
	  }
	  //otherwise copypaste default message and print
	  cprintf("pid %d %s: trap %d err %d on cpu %d "
	              "eip 0x%x addr 0x%x--kill proc\n",
	              proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip,
	              rcr2());
	  proc->killed = 1;
	  exit();
  case T_IRQ0 + IRQ_TIMER:
  /*
  	 for (i = 0; i < NPROC; i++)
  	 {
  		 struct proc* currentProc = getProc(i);
  		 if (currentProc->signalhandlers[SIGALRM] > -1)
  		 {
  			 if (currentProc && currentProc->alarmCounter > 0)
			 {
				  currentProc->alarmCounter--;
				  if (currentProc->alarmCounter == 0)
				  {
					  siginfo_t sigalrmInfo;			//set new siginfo for SIGALRM
					  sigalrmInfo.signum = SIGALRM;
					  *((siginfo_t*)(currentProc->tf->esp - 4)) = sigalrmInfo;
					  currentProc->tf->esp = currentProc->tf->esp - 8;
					  currentProc->tf->eip = (uint) currentProc->signalhandlers[1];
				  }
			  }
  		 }
  	 }
*/
    if(cpu->id == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpu->id, tf->cs, tf->eip);
    lapiceoi();
    break;
   
  //PAGEBREAK: 13
  default:
    if(proc == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpu->id, tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip, 
            rcr2());
    proc->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running 
  // until it gets to the regular system call return.)
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();
  //for (i = 0; i < NPROC; i++)
  //{
	  //struct proc* currentProc = getProc(i);
	  if (proc && proc->alarmCounter > 0)
	  {
	 	 if (proc->signalhandlers[SIGALRM] > -1)
	 	 {
	 		  proc->alarmCounter -= 25;
	 		  if (proc->alarmCounter == 0)
	 		  {
	 			  cprintf("reached here\n");
	 			  siginfo_t sigalrmInfo;			//set new siginfo for SIGALRM
	 			  sigalrmInfo.signum = SIGALRM;
	 			  *((siginfo_t*)(proc->tf->esp - 4)) = sigalrmInfo;
	 			  proc->tf->esp = proc->tf->esp - 8;
	 			  proc->tf->eip = (uint) proc->signalhandlers[1];
	 		  }
	 	 }
	  }
  //}
}
