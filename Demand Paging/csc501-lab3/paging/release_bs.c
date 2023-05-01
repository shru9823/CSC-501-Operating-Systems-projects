#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) {

  /* release the backing store with ID bs_id */
//   kprintf("release_bs To be implemented!\n");
  STATWORD ps;
	disable(ps);

	if (bs_id<0 || bs_id>=NBS || bsm_tab[bs_id].bs_status == BSM_UNMAPPED) {
		restore(ps);
		return(SYSERR);
	}
  if(bsm_tab[bs_id].ref_cnt >= 2) {
		restore (ps);
		return(SYSERR);
	}

  if (bsm_tab[bs_id].bs_pid[currpid] == currpid && (bsm_tab[bs_id].is_private == 1 || bsm_tab[bs_id].ref_cnt < 2)) {
		free_bsm(bs_id);
	}

  restore(ps);
  return OK;

}

