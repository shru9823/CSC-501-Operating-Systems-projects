#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

// void update_proc_priority (int pid)
// {
// 	int i, j, maxprio;
// 	struct lentry *lptr;
//     struct pentry *pptr = &proctab[pid];
//     maxprio = 0;
// 	for (j = 0; j < NLOCKS; ++j) {
// 		if (pptr->plock[j] != LFREE) {
// 			maxprio = (maxprio < ltable[j].lpprio) ? ltable[j].lpprio : maxprio;
// 		}	
// 	}
// 	proctab[ppid].pprio = maxprio;
// }

int check_waittime(int pid, int priority, int ldes1) 
{
    if (locktab[ldes1].lqtype[pid] == WRITE) {
        return pid;
    }
    while(!isbadpid(q[pid].qprev) && q[q[pid].qprev].qkey == priority){
        if (locktab[ldes1].lqtype[q[pid].qprev] == WRITE && ((proctab[pid].lqwaittime - proctab[q[pid].qprev].lqwaittime ) < 500 || (proctab[pid].lqwaittime - proctab[q[pid].qprev].lqwaittime ) > -500)){
            return (q[pid].qprev);
        }
        pid = q[pid].qprev;
    }
    return pid;
}

void release_lock(int ldes1) 
{
    struct lentry *lptr;
    lptr = &locktab[ldes1];

    if(q[lptr->lqhead].qnext == lptr->lqtail) {
        lptr->ltype = EXISTS;
        return;
    } 
    else{
        int newproc, newprio;
        newproc = q[lptr->lqtail].qprev;
        newprio = q[newproc].qkey;

        newproc = check_waittime(newproc, newprio, ldes1);
        newproc = dequeue(newproc);
        ready(newproc, RESCHNO);
        
        lptr->lockcnt = 1;
        lptr->ltype = lptr->lqtype[newproc];
        lptr->lockprio = newprio;

        proctab[newproc].lwaitret = OK;
        newproc = q[lptr->lqtail].qprev;
        newprio = q[newproc].qkey;
        if (lptr->ltype == READ) {	
			while (newproc != lptr->lqhead && lptr->lqtype[newproc] == READ) {	
				newproc = dequeue(newproc);
				ready(newproc, RESCHNO);
				lptr->lockcnt = lptr->lockcnt + 1;
                lptr->lqtype[newproc] = LFREE;
				lptr->lqwait[newproc] = BADPID;
				proctab[newproc].lwaitret = OK;
                newproc = q[newproc].qprev;
				newprio = q[newproc].qkey;
			}
		}
    }
}

// void updateholdprio (int ldes)
// {
//     struct lentry *lptr;    
//     struct pentry *pptr;
//     int i, mpid, pprio;
//     lptr = &ltable[ldes];
    
//     if (lptr->lpprio == -1) {
//         lptr->lpprio = gethighprio (ldes);
//     }

//     for (i = 0; i < NPROC; ++i) {
//         if (lptr->lholdprocs[i] != BADPID) {
            
//             proctab[i].pprio = lptr->lpprio;
//             // kprintf("pprio: %d ",proctab[i].pprio);
//             // kprintf(" -> %d ",proctab[i].pinh);
//             pptr = &proctab[i];
            
//             mpid = checkpriority(ldes, i, pptr->pprio);
//             // kprintf("mpid: %d\n", mpid);
//             if (mpid != -1) {
//                 /* priority inheritance case */
//                 inheritprio (ldes, mpid, i);
//             }
//         }
//     }
// }

void update_proc_priority (int pid)
{
	int i, j, maxprio;
    struct pentry *pptr;
    struct lentry *lptr;
    pptr = &proctab[pid];
    maxprio = 0;
	for (j = 0; j < NLOCKS; ++j) {
		if (pptr->plocks[j] != LFREE) {
			maxprio = (maxprio < locktab[j].lprio) ? locktab[j].lprio : maxprio;
		}	
	}
	proctab[pid].pprio = maxprio;
}

int releaseall(int numlocks, int ldes1, ...){
    STATWORD ps;
    disable (ps);
    struct lentry *lptr;
    int curlock;
    int i;
    int ret = OK;
    for(i=0; i<numlocks; i++){
        curlock = *(&ldes1 + i);
        lptr = &locktab[curlock];
        if(isbadlock(curlock) || locktab[curlock].lstate == LFREE || locktab[curlock].ltype == EXISTS){
            ret = SYSERR;
            continue;
        }

        if(locktab[curlock].ltype == WRITE){
            lptr->lockcnt = 0;
            lptr->lockprio = 0;
            release_lock(curlock);
        }
        else if(locktab[curlock].ltype == READ){
            lptr->lockcnt = lptr->lockcnt - 1;
            if (lptr->lockcnt == 0) {
                release_lock(curlock);
            }
        }
        lptr->lholdprocs[currpid] = BADPID;
		proctab[currpid].plocks[curlock] = LFREE;
    }
    update_proc_priority (currpid);
    restore (ps);
    return(ret);
}