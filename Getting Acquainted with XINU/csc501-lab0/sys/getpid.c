/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern int is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL getpid()
{
	// Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 2;
	start_time = ctr1000;
	if (is_tracing_syscall) {
		end_time = ctr1000;
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_getpid";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

	return(currpid);
}
