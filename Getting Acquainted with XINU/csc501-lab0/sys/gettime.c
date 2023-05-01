/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <date.h>
#include <lab0.h>

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern int is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */

    // Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 4;
	start_time = ctr1000;
	if (is_tracing_syscall) {
		end_time = ctr1000;
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_gettime";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

    return OK;
}
