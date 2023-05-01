#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

unsigned long reg;

SYSCALL init_page_directory(int pid)
{
	int frame, status;
	status = get_frm (&frame);
	if (status == SYSERR){
		return(SYSERR);
	}

	//	init frame_tab
    frm_tab[frame].fr_dirty = 0;
	frm_tab[frame].fr_pid = pid;
	frm_tab[frame].fr_status = FRM_MAPPED;
	frm_tab[frame].fr_type = FR_DIR;
	frm_tab[frame].fr_vpno = -1;
	frm_tab[frame].fr_refcnt = 4;

	proctab[pid].pdbr = (FRAME0 + frame) * NBPG;
    int i;
    for (i = 0; i < 1024; i++) {
		pd_t *pd_entry = (pd_t*)(proctab[pid].pdbr + (i * sizeof(pd_t)));
		pd_entry->pd_write = 1;
        pd_entry->pd_acc = 0;
		pd_entry->pd_mbz = 0;
		pd_entry->pd_fmb = 0;
		pd_entry->pd_global = 0;
		pd_entry->pd_avail = 0;
		pd_entry->pd_base = 0;
		pd_entry->pd_pres = 0;
		pd_entry->pd_user = 0;
		pd_entry->pd_pwt = 0;
        pd_entry->pd_pcd = 0;

		if (i < 4) {
            pd_entry->pd_pres = 1;
			pd_entry->pd_base = FRAME0 + i;
			pd_entry->pd_write = 1;
			// frm_tab[frame].fr_refcnt += 1;
		}
	}

	return(OK);
}


SYSCALL init_global_pagetables(int pid)
{
	int i, f;
	int frame, status;
	int frame_base;
	for (i = 0; i < 4; i++) {
		if(get_frm(&frame) == SYSERR){	
			return(SYSERR);
		}
		
		frm_tab[frame].fr_type = FR_PAGE;
		frame_base = frame + FRAME0;
		frm_tab[frame].fr_status = FRM_MAPPED;
		frm_tab[frame].fr_pid = pid; 
		
		for (f= 0; f < 1024; f++) {
			pt_t *pte = (pt_t*)(frame_base*NBPG + (f * (sizeof(pt_t))));
			init_pte (pte);
			pte->pt_pres = 1;
			pte->pt_write = 1;
			pte->pt_base = f + (i * FRAME0);
			frm_tab[frame].fr_refcnt += 1;
		} 	
	}
	return(OK);		
}


void init_pte(pt_t *pte)
{
	pte->pt_pres = 0;
	pte->pt_write = 1;
	pte->pt_user = 0;
	pte->pt_pwt = 0;
	pte->pt_pcd = 0;
	pte->pt_acc = 0;
	pte->pt_dirty = 0;
	pte->pt_mbz = 0;        
	pte->pt_global = 0;
	pte->pt_avail = 0;
	pte->pt_base = 0;	
}

int get_new_pagetable(int pid)
{
	int page_frame;
	int status, pagetable_frame;
	status = get_frm(&page_frame);
	if (status == SYSERR){
		return(SYSERR);
	}

	pagetable_frame = FRAME0 + page_frame;
	init_frame(page_frame);
	map_frm(page_frame, pid, FR_PAGE);
	
	pt_t *pt_entry;
	int frame;
	for (frame = 0; frame < 1024; frame++) {
		pt_entry = (pt_t*)(pagetable_frame * NBPG + frame * sizeof(pt_t));
		pt_entry->pt_pres = 0;
		pt_entry->pt_write = 1;
		pt_entry->pt_user = 0;
		pt_entry->pt_pwt = 0;
		pt_entry->pt_pcd = 0;
		pt_entry->pt_acc = 0;
		pt_entry->pt_dirty = 0;
		pt_entry->pt_mbz = 0;        
		pt_entry->pt_global = 0;
		pt_entry->pt_avail = 0;
		pt_entry->pt_base = 0;
		
	}		
	return page_frame;
}