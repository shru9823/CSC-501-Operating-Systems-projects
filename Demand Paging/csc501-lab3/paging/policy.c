/* policy.c = srpolicy*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>


extern int page_replace_policy;
/*-------------------------------------------------------------------------
 * srpolicy - set page replace policy 
 *-------------------------------------------------------------------------
 */
SYSCALL srpolicy(int policy)
{
  /* sanity check ! */
//   kprintf("To be implemented!\n");
  page_replace_policy = policy;
  return OK;
}

/*-------------------------------------------------------------------------
 * grpolicy - get page replace policy 
 *-------------------------------------------------------------------------
 */
SYSCALL grpolicy()
{
  return page_replace_policy;
}

void insert_frame_FIFO(int fr) {

//	kprintf("Inserted page in FIFO Queue as FIFO is page replacement policy %d\n", i);
	FifoQueue *frame = (FifoQueue *) getmem(sizeof(FifoQueue));
	frame->frameID = fr;
	frame->nextFrame = head_FIFO.nextFrame;
	head_FIFO.nextFrame = frame;
	n_pages_FIFO = n_pages_FIFO + 1;
}

void insert_frame_SC(int fr) {

//	kprintf("Inserted page in FIFO Queue as FIFO is page replacement policy %d\n", i);
	SCQueue *frame = (SCQueue *) getmem(sizeof(SCQueue));
	frame->frameID = fr;
	frame->nextFrame = NULL;
	curr_head->nextFrame = frame;
	curr_head = curr_head->nextFrame;
	n_pages_SC = n_pages_SC + 1;
}


int getFrame_FIFO() {

	int id;
	FifoQueue *hd = &head_FIFO;
	FifoQueue *prev = &head_FIFO;

	for (id = 0; id < n_pages_FIFO; id++) {
		prev = hd;
		hd = hd->nextFrame;
	}

	prev->nextFrame = NULL;

	freemem(hd, sizeof(FifoQueue));
	if (n_pages_FIFO > 0)
		n_pages_FIFO--;

	free_frm(hd->frameID + FRAME0);
	kprintf("Frame %d is replaced.\n", hd->frameID);
	freemem(hd, sizeof(FifoQueue));
	return hd->frameID;
}
