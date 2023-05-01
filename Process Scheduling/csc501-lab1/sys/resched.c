/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>
#include <stdio.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	// code change
	int sched_class = getschedclass();
	
	if(sched_class == AGESCHED){
		int next_proc = q[rdyhead].qnext;
		while(next_proc != rdytail){
			if(next_proc != 0){
				chprio(next_proc, (&proctab[next_proc])->pprio+1);
				q[next_proc].qkey += 1;
			}
			next_proc = q[next_proc].qnext;
		}
			
	}
	else if(sched_class == LINUXSCHED){
		optr= &proctab[currpid];
		int currprio = optr->goodness - optr->counter;
		optr->counter = preempt;

		if(optr->counter > 0){
			optr->goodness = (optr->counter) + currprio;
		}
		else{
			optr->goodness = 0;
			optr->counter = 0;
		}

		/* force context switch */
		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}
	
		// check if end of the epoch
		if(is_end_of_epoch() == 1){
			init_new_epoch();
		}
		
		// get process with highest goodness
		int new_process = get_next_process();
	
		currpid = dequeue(new_process);
		nptr = &proctab[currpid];
		nptr->pstate = PRCURR;
		// kprintf("curr proc - %d\n", new_process);
		if(new_process != NULLPROC){
	
#ifdef	RTCLOCK
		preempt = nptr->counter;		/* reset preemption counter	*/
#endif
		}
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
		return OK;
	}
	// end code change

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}
