#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <q.h>
#include <stdio.h>

struct	lentry	locktab[NLOCKS];	/* lock table			*/
int	nextlock;

void linit(){
    int i,j;
    struct	lentry	*lptr;
    nextlock = NLOCKS-1;
    for (i=0 ; i<NLOCKS ; i++) {	/* initialize locks */
        (lptr = &locktab[i])->lstate = LFREE;
        lptr->lprio = 0;
        lptr->lockprio = 0;
        lptr->ltype = DELETED;
        lptr->lockcnt = 0;
        lptr->lqtail = 1 + (lptr->lqhead = newqueue());
        for (j = 0; j < NPROC; j++) {
            lptr->lholdprocs[j] = BADPID;
            lptr->lqwait[j] = BADPID;
            lptr->lqtype[j] = DELETED;
        }
    }
}