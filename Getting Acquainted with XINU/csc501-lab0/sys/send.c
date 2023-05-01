/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL	send(int pid, WORD msg)
{

	// Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 12;
	start_time = ctr1000;
	// end

	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);

		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_send";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end

		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);

	// Code added to trace syscall
	end_time = ctr1000;
	if (is_tracing_syscall) {
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_send";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

	return(OK);
}
