/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL scount(int sem)
{

	// Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 10;
	start_time = ctr1000;
	// end

extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE){
		
		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_scount";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end
		
		return(SYSERR);
	}

	// Code added to trace syscall
	end_time = ctr1000;
	if (is_tracing_syscall) {
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_scount";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

	return(semaph[sem].semcnt);
}
