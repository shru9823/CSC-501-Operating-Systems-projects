/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{

	// kprintf("pfint To be implemented!\n");
	STATWORD ps;
	disable(ps);

	int store, pageth;
	unsigned long faulted_address;
	faulted_address = read_cr2 ();


	virt_addr_t *v_addr = (virt_addr_t*)&faulted_address;
	pd_t *pd = (pd_t*)(proctab[currpid].pdbr + (sizeof(pd_t)) * (v_addr->pd_offset)); //curr page directory

	if ((bsm_lookup(currpid, faulted_address, &store, &pageth) == SYSERR) || (store == -1) || (pageth == -1)) {
		kill(currpid);
		restore(ps);
		return (SYSERR);
	}

	int new_frame;
	if (pd->pd_pres == 0) {
		new_frame = get_new_pagetable(currpid);
		// kprintf("get_new_pgtable!\n");
		if (new_frame == -1) {
			return SYSERR;
		}
		pd->pd_pres = 1;
		pd->pd_write = 1;
		pd->pd_base = FRAME0 + new_frame;
		frm_tab[new_frame].fr_type = FR_TBL;
		frm_tab[new_frame].fr_refcnt += 1;
	}	

	pt_t *pt_entry = (pt_t*)((pd->pd_base)*NBPG + (sizeof(pt_t)) * (v_addr->pt_offset));

	int status;
	int pt_frame;
	if (pt_entry->pt_pres == 0) {
		status = bsm_lookup(currpid, faulted_address, &store, &pageth);
		// kprintf("bsm_lookup!\n");
		if(status == (SYSERR)) {
				kill(currpid);
				kprintf("%d\n", currpid);
				kprintf("kill!\n");
				restore(ps);
				return(SYSERR);
		}
		status = get_frm(&pt_frame);
		// kprintf("get_frm!\n");
		if (status == (SYSERR)) {
			restore(ps);
			return(SYSERR);
		}

		pt_entry->pt_pres = 1;
		pt_entry->pt_write = 1;
		pt_entry->pt_base = pt_frame + FRAME0;

	} 

	frm_tab[pt_frame].fr_vpno = faulted_address/NBPG;
	frm_tab[pt_frame].fr_type = FR_PAGE;
	frm_tab[pt_frame].fr_refcnt += 1;
	frm_tab[pt_frame].fr_dirty = 0;
	frm_tab[pt_frame].fr_status = FRM_MAPPED;	
	frm_tab[pt_frame].fr_pid = currpid;
	

	pt_entry->pt_base = pt_frame + FRAME0;
	frm_tab[(unsigned int) pt_entry / NBPG - FRAME0].fr_refcnt++;
	pt_entry->pt_pres = 1;
	pt_entry->pt_write = 1;

	// pt_entry->pt_acc = 1;

	if (page_replace_policy == FIFO)
		insert_frame_FIFO(pt_frame);
	// else if (page_replace_policy == LRU) {
	// 	LRU_updateTimeCount();
	// }



	read_bs ((char*)((pt_frame + FRAME0) * NBPG), store, pageth);

	write_cr3 (proctab[currpid].pdbr);

	restore (ps);
	return OK;
}


