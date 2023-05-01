/* vgetmem.c - vgetmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>
#include <paging.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 * vgetmem  --  allocate virtual heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
WORD	*vgetmem(nbytes)
	unsigned nbytes;
{

	// kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);

	struct pentry *pptr;
	pptr = &proctab[currpid];
	if (nbytes == 0 || ((pptr->vmemlist->mnext) == NULL)) {
		kprintf("nbytes problem!\n");
		restore(ps);
		return((WORD*)SYSERR);
	}

	struct mblock *next, *curr;
	struct mblock *rem;
	nbytes = (unsigned int)roundmb(nbytes);
	
	for (curr = (pptr->vmemlist), next = (pptr->vmemlist->mnext);
		next != (struct mblock *)NULL; curr = next, next = next->mnext) {

		if (next->mlen > nbytes) {
			rem = (struct mblock *)( (unsigned)next + nbytes);
			curr->mnext = rem;
			rem->mnext = next->mnext;
			rem->mlen = next->mlen - nbytes;
			restore(ps);
			return((WORD*)next);
		}
		else if (next->mlen == nbytes) {
			curr->mnext = next->mnext;
			restore(ps);
			return((WORD*)next);
		}
	}
	restore(ps);
	return((WORD*)SYSERR);
}




// /* vgetmem.c - vgetmem */

// #include <conf.h>
// #include <kernel.h>
// #include <mem.h>
// #include <proc.h>
// #include <paging.h>

// extern struct pentry proctab[];
// /*------------------------------------------------------------------------
//  * vgetmem  --  allocate virtual heap storage, returning lowest WORD address
//  *------------------------------------------------------------------------
//  */
// WORD	*vgetmem(nbytes)
// 	unsigned nbytes;
// {

// 	// kprintf("vgetmem To be implemented!\n");
// 	STATWORD ps;
// 	disable(ps);

// 	struct pentry *pptr;
// 	pptr = &proctab[currpid];
// 	if (nbytes == 0 || ((pptr->vmemlist->mnext) == (struct mblock *)NULL)) {
// 		kprintf("nbytes problem!\n");
// 		restore(ps);
// 		return((WORD*)SYSERR);
// 	}
// 	kprintf("problem here !\n");
// 	struct mblock *next, *curr;
// 	struct mblock *rem;
// 	nbytes = (unsigned int)roundmb(nbytes);
// 	// pptr = &proctab[currpid];

// 	curr = pptr->vmemlist;
// 	next = pptr->vmemlist->mnext;
// 	while (next != NULL) {
// 		kprintf("problem for loop !\n");
// 		kprintf("next->mlen %u", next->mlen);
// 		// if (next->mlen > nbytes) {
// 		// 	rem = (struct mblock *)( (unsigned)next + nbytes);
// 		// 	rem->mnext = next->mnext;
// 		// 	rem->mlen = next->mlen - nbytes;
// 		// 	curr->mnext = rem;
// 		// 	kprintf("problem word!\n");
// 		// 	restore(ps);
// 		// 	return((WORD*)next);
// 		// }
// 		// else if (next->mlen == nbytes) {
// 		// 	curr->mnext = next->mnext;
// 		// 	kprintf("problem else word!\n");
// 		// 	restore(ps);
// 		// 	return((WORD*)next);
// 		// }
// 		if (next->mlen > nbytes) {
// 			rem = next + nbytes;
// 			rem->mnext = next->mnext;
// 			rem->mlen = next->mlen - nbytes;
// 			curr->mnext = rem;
// 			kprintf("problem word!\n");
// 			restore(ps);
// 			return(next);
// 		}
// 		else if (next->mlen == nbytes) {
// 			curr->mnext = next->mnext;
// 			kprintf("problem else word!\n");
// 			restore(ps);
// 			return(next);
// 		}
// 		curr = next;
// 		next = next->mnext;
// 	}
// 	kprintf("problem returning !\n");
// 	restore(ps);
// 	return((WORD*)SYSERR);
// }


