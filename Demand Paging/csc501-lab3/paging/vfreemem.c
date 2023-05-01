/* vfreemem.c - vfreemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>
#include <paging.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 *  vfreemem  --  free a virtual memory block, returning it to vmemlist
 *------------------------------------------------------------------------
 */
SYSCALL	vfreemem(block, size)
	struct	mblock	*block;
	unsigned size;
{
	// kprintf("vfreemem To be implemented!\n");
	STATWORD ps;
	disable (ps);

	struct pentry *pptr;
	if ((unsigned)block < (unsigned)(4096 * NBPG) || (unsigned)block > (unsigned)(pptr->vhpno + pptr->vhpnpages)*NBPG) {
		restore (ps);
		return(SYSERR);
	}
	if (size > (128 * NBPG) || size <= 0) {
		restore(ps);
		return(SYSERR);
	}

	struct mblock *next, *curr;
	unsigned front;
	pptr = &proctab[currpid];
	size = (unsigned int)roundmb(size);
	
	for (next = (pptr->vmemlist->mnext), curr = (pptr->vmemlist); next != (struct mblock *)NULL && next < block; curr = next, next = next->mnext);
	
	front=curr->mlen+(unsigned)curr;
	if ((front >(unsigned)block && curr != (pptr->vmemlist)) ||
            (next != NULL && (size+(unsigned)block) > (unsigned)next)) {
                restore(ps);
                return(SYSERR);
	}

	if (front == (unsigned)block && curr != (pptr->vmemlist)) {
		curr->mlen = curr->mlen + size;
	} 
	else {
		block->mnext = next;
		block->mlen = size;
		curr->mnext = block;
		curr = block;
	}

	if ((unsigned)( curr->mlen + (unsigned)curr) == (unsigned)next) {
		curr->mlen = curr->mlen + next->mlen;
		curr->mnext = next->mnext;
	}
	
	kprintf("reached in end vfreemem\n");

	restore (ps);
	return(OK);
}
