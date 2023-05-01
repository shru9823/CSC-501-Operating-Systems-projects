#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newlock();

int lcreate (void){
    STATWORD ps;
    int loc;

    disable(ps);
    if( (loc = newlock()) == SYSERR){
        restore(ps);
        return(SYSERR);
    }
    restore(ps);
    return(loc);
}

LOCAL int newlock(){
    int i;
    int loc;
    for(i=0; i<NLOCKS; i++){
        loc = nextlock--;
        if(nextlock < 0){
            nextlock = NLOCKS-1;
        }
        if(locktab[loc].lstate == LFREE){
            locktab[loc].lstate = LUSED;
            locktab[loc].ltype = EXISTS;
            // locktab[loc].lprio = 0;
            return(loc);
        }
    }
    return(SYSERR);
}