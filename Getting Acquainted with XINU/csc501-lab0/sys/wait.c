/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL	wait(int sem)
{
	// Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 26;
	start_time = ctr1000;
	// end

	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);

		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_wait";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end

		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);

		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_wait";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end

		return pptr->pwaitret;
	}
	restore(ps);

	// Code added to trace syscall
	end_time = ctr1000;
	if (is_tracing_syscall) {
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_wait";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

	return(OK);
}
