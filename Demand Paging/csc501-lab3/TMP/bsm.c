/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

bs_map_t bsm_tab[NBS];

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{
    STATWORD 	ps;  
    disable(ps);
    int bs,pid;
    
    for (bs=0; bs<NBS; bs++){
        bsm_tab[bs].bs_status = BSM_UNMAPPED;
        bsm_tab[bs].bs_npages = 0;
        bsm_tab[bs].bs_sem = 0;
        bsm_tab[bs].is_private = 0;
        for (pid=0; pid<NPROC; pid++){
            bsm_tab[bs].bs_pid[pid] = BADPID;
            bsm_tab[bs].bs_vpno[pid] = 4096;
        } 
        bsm_tab[bs].ref_cnt = 0;
    }

    restore(ps);
    return(OK);
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{
    STATWORD 	ps;  
    disable(ps);
    int bs;

    for (bs=0; bs<NBS; bs++){
        if (bsm_tab[bs].bs_status == BSM_UNMAPPED){
            *avail = bs; 
            break;
        }
    }

    if (bs >= NBS){             /* No backing store available */
        *avail = -1;
        restore(ps);
        return(SYSERR);
    }
    restore(ps);
    return(OK);
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
    STATWORD 	ps;  
    disable(ps);
    int pid;

    if (bsm_tab[i].ref_cnt != 0) {
        restore(ps);
		return(SYSERR);
	}

    if (i<0 || i>=NBS){
        restore(ps);
        return(OK);
    }
    
    bsm_tab[i].bs_status = BSM_UNMAPPED;
    bsm_tab[i].bs_npages = 0;
    bsm_tab[i].bs_sem = 0;
    bsm_tab[i].is_private = 0;
    for (pid=0; pid<NPROC; pid++){
        bsm_tab[i].bs_pid[pid] = BADPID;
        bsm_tab[i].bs_vpno[pid] = 4096;
    } 
    bsm_tab[i].ref_cnt = 0;

    restore(ps);
    return(OK);
}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
    STATWORD ps;
    disable (ps);
	long vpno;
	vpno = vaddr / NBPG;
    int bs;
	for (bs=0; bs<NBS; bs++) {
		if (bsm_tab[bs].bs_status == BSM_MAPPED && pid == bsm_tab[bs].bs_pid[pid] && vpno >= bsm_tab[bs].bs_vpno[pid]){
            *pageth = vpno - bsm_tab[bs].bs_vpno[pid];
            *store = bs;
            restore(ps);
            return(OK); 
		}
	}
    *store = -1;
	*pageth = -1;
	restore (ps);
	return(SYSERR);   
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
    STATWORD ps;
    disable(ps);
    if (vpno<NBPG || source<0 || source>=NBS){
        return(SYSERR);
    }
    if (npages<=0 || npages>128){
        return(SYSERR);
    }

    // int bs;
    if (bsm_tab[source].bs_status == BSM_UNMAPPED){
		bsm_tab[source].bs_status = BSM_MAPPED;
		bsm_tab[source].bs_npages = npages;
	}

    bsm_tab[source].bs_pid[pid] = pid;
    bsm_tab[source].bs_vpno[pid] = vpno;
    bsm_tab[source].ref_cnt += 1;	
    bsm_tab[source].bs_sem = 0;	
    bsm_tab[source].bs_npages = npages;
    
    restore(ps);
    return(OK);
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
    
    STATWORD ps;
    disable(ps);
	int store, pageth;
	int t_store, t_page;
	long vaddr;
	vaddr = vpno * NBPG;
    int status;
	status = bsm_lookup (pid, vaddr, &store, &pageth);
	if (status == SYSERR) {
		restore(ps);
		return(SYSERR);
	}
    if (flag == 1) {
		proctab[pid].vmemlist = NULL;
    }

    int frame;
	for (frame=0; frame < NFRAMES; frame++) {
		if (frm_tab[frame].fr_pid==pid && frm_tab[frame].fr_type==FR_PAGE && bsm_lookup(pid,frm_tab[frame].fr_vpno*NBPG,&t_store,&t_page) != SYSERR) {
            if(store == t_store){
                write_bs((frame+FRAME0)*NBPG,store,t_page); 
                free_frm(frame);
            }
		}
	}
	bsm_tab[store].ref_cnt = bsm_tab[store].ref_cnt - 1;
	if (bsm_tab[store].ref_cnt <= 0){
		free_bsm(store);
    }
    
    bsm_tab[store].bs_vpno[pid] = 4096;
	bsm_tab[store].bs_pid[pid] = BADPID;

	restore(ps);
	return(OK);
}


