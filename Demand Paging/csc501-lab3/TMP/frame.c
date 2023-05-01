/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

fr_map_t frm_tab[NFRAMES];

/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_frm()
{
//   kprintf("init_frm - To be implemented!\n");
  STATWORD ps;
  disable(ps);
	int frame;
	for (frame=0; frame<NFRAMES; frame++) {
		frm_tab[frame].fr_status = FRM_UNMAPPED;
		frm_tab[frame].fr_pid = BADPID;
		frm_tab[frame].fr_vpno = -1;
		frm_tab[frame].fr_refcnt = 0;
		frm_tab[frame].fr_type = FR_TBL;
		frm_tab[frame].fr_dirty = 0;
	} 
	restore(ps);
  return(OK);
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frm(int* avail)
{
//   kprintf("get_frm To be implemented!\n");
  STATWORD ps;
	disable(ps);
  int frame;
	for (frame=0; frame<NFRAMES; frame++) {
		if (frm_tab[frame].fr_status == FRM_UNMAPPED) {
			*avail = frame;
			restore(ps);
			return(OK);
		}
	}
  	kprintf("Page Replacement Policy %d\n",grpolicy());
	if (grpolicy() == FIFO) {
		*avail = getFrame_FIFO();
	}
	// if (grpolicy() == SC) {
	// 	*avail = getFrame_FIFO();
	// }
	// if (*avail == -1){
	// 	restore(ps);
	// 	return(SYSERR);
	// }

	restore(ps);
	return(SYSERR);
}

/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */
SYSCALL free_frm(int i)
{
//   kprintf("free_frm To be implemented!\n");
  frm_tab[i].fr_status = FRM_UNMAPPED;
  frm_tab[i].fr_pid = BADPID;
  frm_tab[i].fr_vpno = -1;
  frm_tab[i].fr_refcnt = 0;
  frm_tab[i].fr_type = FR_TBL;
  frm_tab[i].fr_dirty = 0;
  return OK;
}

void map_frm (int pageframe, int pid, int type)
{
	if (pageframe < 0 || pageframe >= NFRAMES){
		return(SYSERR);
	}
	frm_tab[pageframe].fr_status = FRM_MAPPED;
	frm_tab[pageframe].fr_pid = pid;
	frm_tab[pageframe].fr_refcnt += 1;
	frm_tab[pageframe].fr_type = type;	
}

void init_frame(int frame)
{
	frm_tab[frame].fr_status = FRM_UNMAPPED;
	frm_tab[frame].fr_pid = BADPID;
	frm_tab[frame].fr_vpno = -1;
	frm_tab[frame].fr_refcnt = 0;
	frm_tab[frame].fr_type = FR_TBL;
	frm_tab[frame].fr_dirty = 0;
}

int write2CR3(int pid) {
	unsigned int pdbr = (proctab[pid].pdbr) / NBPG;
	if ((frm_tab[pdbr - FRAME0].fr_status != BSM_MAPPED) || (frm_tab[pdbr - FRAME0].fr_pid != pid) || (frm_tab[pdbr - FRAME0].fr_type != FR_DIR)) {
		return SYSERR;
	}

	write_cr3(proctab[pid].pdbr);
	return OK;
}

