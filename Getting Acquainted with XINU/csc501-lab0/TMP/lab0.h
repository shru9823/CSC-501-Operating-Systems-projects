#ifndef _lab0_h_
#define _lab0_h_

#include <stdio.h>

// Task 1
extern long zfunction(long);

// Task 2
extern void printsegaddress();

// Task 3
extern void printtos();

//Task 4
extern void printprocstks(int priority);

//Task 5
extern int is_tracing_syscall;	/* true if tracing is on else false*/
extern unsigned long ctr1000;	/* global time variable */

extern struct sys_trace {		/* structure to maintain the syscall trace */
	char* syscall_name;		    /* name of the syscall */
	long frequency;		        /* number of times a syscall is called */
	unsigned long total_time;   /* total time taken for execution */
};

// extern struct sys_trace sys_trace_table[50][27];  /* Table to store sys_trace for each syscall*/

extern void syscallsummary_start();
extern void syscallsummary_stop();
extern void printsyscallsummary();


#endif
