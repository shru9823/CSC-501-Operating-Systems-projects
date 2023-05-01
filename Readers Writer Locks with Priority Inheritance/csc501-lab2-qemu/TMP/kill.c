/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

// void update_lmax_prio(int ldes1)
// {
//     struct lentry *lptr;
//     int maxprio = 0;
//     int i;
// 	int newproc;
//     lptr = &locktab[ldes1];
//     if (isempty(lptr->lqhead)) {
//         lptr->lprio = 0;
//         return;
//     }
//     newproc = q[lptr->lqtail].qprev;
//     while (newproc != lptr->lqhead) {
//         if (maxprio < proctab[newproc].pprio){
//             maxprio = proctab[newproc].pprio;
// 		}
//         newproc = q[newproc].qprev;
//     }
//     lptr->lprio = maxprio;
// } 

// void inherit_priority(int ldes1, int p2, int p1){
//     int pid;
//     // if (proctab[p2].pinh == 0)
//         // proctab[p2].pinh = proctab[p2].pprio;
//     if (proctab[p1].pinh == 0)
//         proctab[p2].pprio = proctab[p1].pprio;
//     else 
//         proctab[p2].pprio = proctab[p1].pinh;
//     proctab[p2].pinh = proctab[p2].pprio;

//     if (proctab[p2].lockid != -1) {
//         pid = check_priority_change(proctab[p2].lockid, p2, proctab[p2].pprio);
//         if (pid != -1){
//             inherit_priority(proctab[p2].lockid, pid, p2);
//         }
//     }
// }

// int check_priority_change(int ldes1, int ppid, int priority){
//     int pid, prio;
    
//     for(pid=0; pid<NPROC; pid++){
//         if(pid != ppid && proctab[pid].pstate != PRFREE){
//             if(proctab[pid].pinh == 0){
//                 prio = proctab[pid].pprio;
//             }
//             else{
//                 prio = proctab[pid].pinh;
//             }
//             if(prio < priority && proctab[pid].plocks[ldes1] != LFREE){
//                 return pid;
//             }
//         }
//     }
//     return -1;
// }

// int get_high_prio(int ldes1)
// {
//     int i;
//     struct lentry *lptr ;
// 	lptr = &locktab[ldes1];
//     for (i = 0; i < NPROC; i++) {
//         if (lptr->lholdprocs[i] != BADPID) {
//             return (proctab[i].pinh);
//         }
//     }
//     return 0;
// }

// void update_phold_prio(int ldes1)
// {
//     struct lentry *lptr;    
//     struct pentry *pptr;
//     int i, pid, pprio;
//     lptr = &locktab[ldes1];
    
//     if (lptr->lprio == -1) {
//         lptr->lprio = get_high_prio(ldes1);
//     }

//     for (i = 0; i < NPROC; i++) {
//         if (lptr->lholdprocs[i] != BADPID) {
            
//             proctab[i].pprio = lptr->lprio;
//             pptr = &proctab[i];
            
//             pid = check_priority_change(ldes1, i, pptr->pprio);

//             if (pid != -1) {
//                 inherit_priority(ldes1, pid, i);
//             }
//         }
//     }
// }

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			update_lmax_prio(pptr->lockid);
			update_phold_prio(pptr->lockid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
