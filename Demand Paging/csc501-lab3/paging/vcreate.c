/* vcreate.c - vcreate */
    
#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <paging.h>

/*
static unsigned long esp;
*/

LOCAL	newpid();
/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL vcreate(procaddr,ssize,hsize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	hsize;			/* virtual heap size in pages	*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	long	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	// kprintf("vcreate To be implemented!\n");
	STATWORD ps;
	disable(ps);

	bsd_t bs;
	int status;	
	status = get_bsm(&bs);
	if(status == SYSERR){
		restore(ps);
		return(SYSERR);
	}

	int pid = create(procaddr, ssize, priority, name, nargs, args);
	if(pid == SYSERR){
		restore(ps);
		return(SYSERR);
	}
	kprintf("pid created %d!\n", pid);
	if (bsm_map(pid, 4096, bs, hsize) == SYSERR) {
		restore(ps);
		return SYSERR;
	}

	bsm_tab[bs].is_private = 1;

	proctab[pid].store = bs;
	proctab[pid].vhpno = 4096;
	proctab[pid].vhpnpages = hsize;

	proctab[pid].vmemlist = getmem(sizeof(struct mblock));
	proctab[pid].vmemlist->mlen = hsize * NBPG;
	// proctab[pid].vmemlist->mnext = (struct mblock*)roundmb(4096 *  NBPG);
	proctab[pid].vmemlist->mnext = NULL;

	
	struct mblock * memblock = (unsigned int)((2048 + bs * 128)*NBPG);
    memblock->mlen  = hsize*NBPG;
	memblock->mnext = NULL;  

	// proctab[pid].vmemlist = memblock;

	restore(ps);
	return pid;
	return OK;
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
