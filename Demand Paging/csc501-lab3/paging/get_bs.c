#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */

  STATWORD 	ps;  
  disable(ps);

  // kprintf("get_bs To be implemented!\n");

  // Handle invalid request
  if (npages <= 0 || npages > 128 || bs_id < 0 || bs_id >= NBS || (bsm_tab[bs_id].is_private == 1 && bsm_tab[bs_id].bs_pid[currpid] == BADPID)){
    restore(ps);
    return(SYSERR);
  }

	if (bsm_tab[bs_id].bs_status == BSM_UNMAPPED) {
		if (bsm_map(currpid, 4096, bs_id, npages) == SYSERR) {
			restore(ps);
			return SYSERR;
		}
		restore(ps);
		return npages;
  } else {
    restore(ps);
    return bsm_tab[bs_id].bs_npages;
  }

}


