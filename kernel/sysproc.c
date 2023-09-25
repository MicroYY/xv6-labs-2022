#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigalarm(void)
{
  int time_interval;
  uint64 cb;

  argint(0, &time_interval);
  argaddr(1, &cb);

  struct proc *p = myproc();
  p->alarm_interval = time_interval;
  p->callback = (void*)cb;

  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc* p = myproc();
  
  p->trapframe->epc = p->irpt.epc;
  p->trapframe->ra = p->irpt.ra;
  p->trapframe->sp = p->irpt.sp;
  p->trapframe->gp = p->irpt.gp;
  p->trapframe->tp = p->irpt.tp;

  p->trapframe->s0 = p->irpt.s0;
  p->trapframe->s1 = p->irpt.s1;
  p->trapframe->s2 = p->irpt.s2;
  p->trapframe->s3 = p->irpt.s3;
  p->trapframe->s4 = p->irpt.s4;
  p->trapframe->s5 = p->irpt.s5;
  p->trapframe->s6 = p->irpt.s6;
  p->trapframe->s7 = p->irpt.s7;
  p->trapframe->s8 = p->irpt.s8;
  p->trapframe->s9 = p->irpt.s9;
  p->trapframe->s10 = p->irpt.s10;
  p->trapframe->s11 = p->irpt.s11;

  p->trapframe->a0 = p->irpt.a0;
  p->trapframe->a1 = p->irpt.a1;
  p->trapframe->a2 = p->irpt.a2;
  p->trapframe->a3 = p->irpt.a3;
  p->trapframe->a4 = p->irpt.a4;
  p->trapframe->a5 = p->irpt.a5;
  p->trapframe->a6 = p->irpt.a6;
  p->trapframe->a7 = p->irpt.a7;

  p->trapframe->t0 = p->irpt.t0;
  p->trapframe->t1 = p->irpt.t1;
  p->trapframe->t2 = p->irpt.t2;
  p->trapframe->t3 = p->irpt.t3;
  p->trapframe->t4 = p->irpt.t4;
  p->trapframe->t5 = p->irpt.t5;
  p->trapframe->t6 = p->irpt.t6;

  return p->trapframe->a0;
}