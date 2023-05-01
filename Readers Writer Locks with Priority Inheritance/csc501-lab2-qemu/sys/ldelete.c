#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int ldelete (int lockdescriptor){
    STATWORD ps;    
	int	pid;
	struct	lentry	*lptr;

	disable(ps);
	if (isbadlock(lockdescriptor) || locktab[lockdescriptor].lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &locktab[lockdescriptor];
	lptr->lstate = LFREE;
	lptr->ltype = DELETED;
	lptr->lockcnt = 0;
	if (nonempty(lptr->lqhead)) {
		while( (pid=getfirst(lptr->lqhead)) != EMPTY)
		  {
		    proctab[pid].lwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	return(DELETED);
}
