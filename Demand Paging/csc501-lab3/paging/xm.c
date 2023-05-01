/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
  // kprintf("xmmap - to be implemented!\n");
  STATWORD 	ps;  
  disable(ps);
  
  if (virtpage<4096 || source<0 || source>=NBS || npages<=0 || npages>NPG){
    restore(ps);
    return(SYSERR);
  }
  
  // check if backing store is private 
  if (bsm_tab[source].is_private == 1 || (bsm_tab[source].bs_status == BSM_MAPPED && npages > bsm_tab[source].bs_npages)){
    restore(ps);
    return(SYSERR);
  }

  if ((bsm_tab[source].bs_status == BSM_UNMAPPED) || (bsm_tab[source].is_private == 1)) {
		restore(ps);
		return OK;
	}

  if (bsm_map(currpid, virtpage, source, npages) == SYSERR) {
		restore(ps);
		return SYSERR;
	}
	// if (bsm_tab[source].bs_status == BSM_UNMAPPED){
	// 	bsm_tab[source].bs_status = BSM_MAPPED;
	// 	bsm_tab[source].bs_npages = npages;
	// }

  // bsm_tab[source].ref_cnt += 1;
  // bsm_tab[source].bs_vpno[currpid] = virtpage;
	// bsm_(tab[source].bs_pid[currpid] = currpid;
	
	restore(ps);
  return(OK);
}



/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage)
{
  // kprintf("xmunmap To be implemented!");
  STATWORD ps;
	disable(ps);
  if (virtpage < 4096) {
		return(SYSERR);
	}

	if(bsm_unmap(currpid, virtpage, 0) == SYSERR){
		restore (ps);
		return(SYSERR);
	}
  write2CR3(currpid);
	restore (ps);
  return SYSERR;
}
