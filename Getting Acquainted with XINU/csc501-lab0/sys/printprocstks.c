#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

unsigned long *esp;

void printprocstks(int priority){
    kprintf("\nTask 4: void printprocstks(int priority)\n");
    int num_of_processes = NPROC;
    struct pentry *process;
    int pid;
    
    for(pid = 0; pid < num_of_processes; pid++){
        process = &proctab[pid];
        if(process->pstate != PRFREE && process->pprio > priority){
            kprintf ("Process [%s]\n", process->pname);
            kprintf ("\tpid: %d\n", pid);
            kprintf ("\tpriority: %d\n", process->pprio);
            kprintf ("\tbase: 0x%08x\n", process->pbase);
            kprintf ("\tlimit: 0x%08x\n", process->plimit);
            kprintf ("\tlen: %d\n", process->pstklen);
            if(pid == currpid){
                asm ("movl %esp,esp");
            }
            else{
                esp = (unsigned long *)process->pesp;
            }
            kprintf ("\tpointer: 0x%08x\n", esp);
        }
    }
}