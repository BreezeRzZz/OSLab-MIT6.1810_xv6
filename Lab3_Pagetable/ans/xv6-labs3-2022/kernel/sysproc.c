#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
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
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 buf;
  int number;
  uint64 bitmask;
  argaddr(0,&buf);
  argint(1,&number);
  argaddr(2,&bitmask);

  //temp mask
  uint64 tempmask = 0;

  // upper limit for number
  if(number>32)
    return -1;

  // get pagetable
  pagetable_t pagetable = myproc()->pagetable;
  
  for(int i=0;i<number;++i)
  { 
    //current page address
    uint64 addr = buf + PGSIZE * i;
    pte_t* pte = walk(pagetable,addr,1);

    if(*pte & PTE_A)
      tempmask = tempmask | (1 << i);

    // clean access bit of PTE
    *pte = *pte & ~PTE_A;
  }

  // using copyout() to copy tempmask to user space
  if(copyout(pagetable,bitmask,(char*)&tempmask,sizeof(tempmask))<0)
    return -1;

  return 0;
}
#endif

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
