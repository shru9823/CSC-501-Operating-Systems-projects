/* lock.h - isbadlock */

#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef	NLOCKS				/* set the number of locks allowed	*/
#define	NLOCKS		50		/*  allowed if not already done	*/
#endif

#define LFREE   '\00'		/* this LOCK is free		    */  
#define LUSED   '\01'       /* this LOCK is used            */ 

#define WRITE   '\02'       /* LOCK required for WRITE operation */
#define READ    '\03'       /* LOCK required for READ operation */
#define EXISTS  '\04'       /* LOCK is valid                */

struct lentry   {           /* LOCK table entry             */
    char lstate;            /* the state LFREE or LUSED		*/
    char ltype;             /* the type whether deleted lock or OK to use the lock */
    int lprio;              /* A priority field indicating the maximum priority among all the processes waiting in the lock’s wait queue   */
    int lockprio;           /* priority for the lock aquired     */
    int lholdprocs[NPROC];  /* processes holding the locks  */
    int lockcnt;            /* count for this LOCK	        */
    int lqhead;             /* q index of head of list		*/
    int lqtail;             /* q index of tail of list		*/
    int lqwait[NPROC];      /* processes waiting for the lock */
    int lqtype[NPROC];      /* type of request READ/WRITE   */
};

extern	struct	lentry	locktab[];
extern	int	nextlock;

#define isbadlock(l)(l<0 || l>=NLOCKS)

extern void linit();
extern int inherit_priority(int ldes, int p2, int p1);
extern int check_priority_change(int ldes, int ppid, int priority);
extern void update_phold_prio (int ldes);
extern void update_lmax_prio (int ldes);

#endif

