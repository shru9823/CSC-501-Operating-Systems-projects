#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

extern int ctr1000;

int is_read_allowed(int ldes1, int priority){
    struct lentry *lptr;
    lptr = &locktab[ldes1];
    int next;
    next = lptr->lqtail;
    while (lptr->lqhead != q[next].qprev) {
            next = q[next].qprev;
            if (lptr->lqtype[next] == WRITE && q[next].qkey >= priority ){
                return 0;       /* read not allowed */
            }
    }
    return 1;                   /* read allowed */
}

// change--------
int inherit_priority(int ldes1, int p2, int p1){
    int pid;
    int npid;

    if (proctab[p2].pinh == 0)
        proctab[p2].pinh = proctab[p2].pprio;
    if (proctab[p1].pinh == 0)
        proctab[p2].pprio = proctab[p1].pprio;
    else 
        proctab[p2].pprio = proctab[p1].pinh;

    if (proctab[p2].lockid != -1) {
        pid = check_priority_change(proctab[p2].lockid, p2, proctab[p2].pprio);
        if (pid != -1){
            inherit_priority(proctab[p2].lockid, pid, p2);
        }
    }
}

int check_priority_change(int ldes1, int ppid, int priority){
    int pid, prio;
    
    for(pid=0; pid<NPROC; pid++){
        if(pid != ppid && proctab[pid].pstate != PRFREE){
            if(proctab[pid].pinh == 0){
                prio = proctab[pid].pprio;
            }
            else{
                prio = proctab[pid].pinh;
            }
            if(prio < priority && proctab[pid].plocks[ldes1] != LFREE){
                return pid;
            }
        }
    }
    return -1;
}

int lock (int ldes1, int type, int priority){
    STATWORD ps;    
	int	pid;
	struct	lentry	*lptr;
    struct	pentry	*pptr;
    pptr = &proctab[currpid];
	disable(ps);
	if (isbadlock(ldes1) || locktab[ldes1].lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}
    if (pptr->lwaitret == DELETED){
        restore(ps);
        return(SYSERR);
    }
	lptr = &locktab[ldes1];

    /*  if LOCK is not owned by any process */
    if(lptr->ltype == EXISTS && lptr->lstate == LUSED){
        lptr->lockcnt = 1;
        lptr->ltype = type; 
        lptr->lholdprocs[currpid] = currpid;
        lptr->lprio = priority;
        lptr->lockprio = priority;
        pptr->plocks[ldes1] = type;
        pptr->lwaitret = OK;
        restore(ps);
        return(OK);
    }
    /*  if LOCK is aquired already  */
    /*  currently WRITE LOCK        */
    else if(lptr->ltype == WRITE){
        int prio;
        if(proctab[currpid].pinh == 0){
            prio = proctab[currpid].pprio;
        }
        else{
            prio = proctab[currpid].pinh;
        }
        int pid ;
        pid = check_priority_change(ldes1, currpid, prio);
        if(pid != -1){
            inherit_priority(ldes1, pid, currpid);
        }
        pptr->pstate = PRWAIT;
        
        lptr->lqwait[currpid] = currpid;
        lptr->lqtype[currpid] = type;
        if (lptr->lprio < pptr->pprio){
            lptr->lprio = pptr->pprio;
        }
        pptr->lockid = ldes1;
        pptr->lqwaittime = ctr1000;
        insert (currpid, lptr->lqhead, priority);
        pptr->lwaitret = DELETED;
        resched();
        restore(ps);
        return pptr->lwaitret;
    }
    /*  currently READ LOCK   */
    else if(lptr->ltype == READ){
        if(type == READ){
            if(!is_read_allowed(ldes1, priority)){
                int prio;
                if(pptr->pinh == 0){
                    prio = pptr->pprio;
                }
                else{
                    prio = pptr->pinh;
                }
                int pid = check_priority_change(ldes1, currpid, prio);
                if(pid != -1){
                    inherit_priority(ldes1, pid, currpid);
                }
                pptr->pstate = PRWAIT;
                lptr->lqwait[currpid] = currpid;
                lptr->lqtype[currpid] = type;
                if (lptr->lprio < pptr->pprio){
                    lptr->lprio = pptr->pprio;
                }
                insert (currpid, lptr->lqhead, priority);
                pptr->lockid = ldes1;
                pptr->lqwaittime = ctr1000;
                pptr->lwaitret = DELETED;
                resched();
                restore(ps);
                return pptr->lwaitret;
            }
            else{
                proctab[currpid].plocks[ldes1] = READ;
                proctab[currpid].lwaitret = OK;
                lptr->lockcnt = lptr->lockcnt + 1;
                lptr->lholdprocs[currpid] = currpid;
                restore (ps);
                return(OK);
            }
        }
        else if(type == WRITE){
            int prio;
            if (pptr->pinh == 0){
                prio = pptr->pprio;
            }
            else{
                prio = pptr->pinh;
            }
            int pid;
            pid = check_priority_change(ldes1, currpid, prio);
            if (pid != -1) {
                inherit_priority(ldes1, pid, currpid);
            }
            pptr->pstate = PRWAIT;
        
            lptr->lqwait[currpid] = currpid;
            lptr->lqtype[currpid] = type;
            if (lptr->lprio < pptr->pprio){
                lptr->lprio = pptr->pprio;
            }
            pptr->lockid = ldes1;
            pptr->lqwaittime = ctr1000;
            insert (currpid, lptr->lqhead, priority);
            pptr->lwaitret = DELETED;
            resched();
            restore(ps);
            return pptr->lwaitret;
        }
    }
    restore(ps);
    return(OK);
}