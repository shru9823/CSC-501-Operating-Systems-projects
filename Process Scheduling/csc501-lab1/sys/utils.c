#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab1.h>

extern int schedclass = 0;

void setschedclass(int sched_class){
    schedclass = sched_class;
}

int getschedclass(){
    return schedclass;
}

int get_next_process(){
    int new_process, next_proc, g_value;
    new_process = NPROC + 1;
    g_value = 0;
    next_proc = q[rdyhead].qnext;
    while(next_proc < NPROC && next_proc != rdytail){
        if((&proctab[next_proc])->goodness >= g_value && (&proctab[next_proc])->goodness > 0){
            new_process = next_proc;
            g_value = (&proctab[next_proc])->goodness;
        }
        next_proc = q[next_proc].qnext;
    }
    if(new_process == NPROC + 1){
        return NULLPROC;
    }
    return new_process;
}

int is_end_of_epoch(){
    int next_proc;
    next_proc = q[rdyhead].qnext;
    while(next_proc != rdytail){
        if((&proctab[next_proc])->goodness > 0){
            return 0;
        }
        next_proc = q[next_proc].qnext;
    } 
    return 1;
}

void init_new_epoch(){
    int pid;
    for(pid=0; pid<NPROC; pid++){
        if((&proctab[pid])->pstate != PRFREE){
            if((&proctab[pid])->counter <= 0){
                (&proctab[pid])->counter = (&proctab[pid])->pprio;
            }
            else{
                (&proctab[pid])->counter = (((&proctab[pid])->counter)/2) + (&proctab[pid])->pprio;
            }
        }
        (&proctab[pid])->goodness = (&proctab[pid])->counter + (&proctab[pid])->pprio;
    }
}
