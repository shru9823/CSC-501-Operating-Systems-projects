/* stacktrace.c - stacktrace */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

static unsigned long	*esp;
static unsigned long	*ebp;

#define STKDETAIL

/*------------------------------------------------------------------------
 * stacktrace - print a stack backtrace for a process
 *------------------------------------------------------------------------
 */

// Code added to trace syscall
extern is_tracing_syscall;
extern unsigned long ctr1000;
extern struct sys_trace sys_trace_table[50][27];
// end

SYSCALL stacktrace(int pid)
{
	// Code added to trace syscall
	unsigned long start_time, end_time;
	const int syscall_id = 23;
	start_time = ctr1000;
	// end

	struct pentry	*proc = &proctab[pid];
	unsigned long	*sp, *fp;

	if (pid != 0 && isbadpid(pid)){
		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_stacktrace";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end
		
		return SYSERR;
	}
	if (pid == currpid) {
		asm("movl %esp,esp");
		asm("movl %ebp,ebp");
		sp = esp;
		fp = ebp;
	} else {
		sp = (unsigned long *)proc->pesp;
		fp = sp + 2; 		/* where ctxsw leaves it */
	}
	kprintf("sp %X fp %X proc->pbase %X\n", sp, fp, proc->pbase);
#ifdef STKDETAIL
	while (sp < (unsigned long *)proc->pbase) {
		for (; sp < fp; sp++)
			kprintf("DATA (%08X) %08X (%d)\n", sp, *sp, *sp);
		if (*sp == MAGIC)
			break;
		kprintf("\nFP   (%08X) %08X (%d)\n", sp, *sp, *sp);
		fp = (unsigned long *) *sp++;
		if (fp <= sp) {
			kprintf("bad stack, fp (%08X) <= sp (%08X)\n", fp, sp);

			// Code added to trace syscall
			end_time = ctr1000;
			if (is_tracing_syscall) {
				sys_trace_table[currpid][syscall_id].syscall_name = "sys_stacktrace";
				sys_trace_table[currpid][syscall_id].frequency += 1;
				sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
			}
			// end

			return SYSERR;
		}
		kprintf("RET  0x%X\n", *sp);
		sp++;
	}
	kprintf("MAGIC (should be %X): %X\n", MAGIC, *sp);
	if (sp != (unsigned long *)proc->pbase) {
		kprintf("unexpected short stack\n");

		// Code added to trace syscall
		end_time = ctr1000;
		if (is_tracing_syscall) {
			sys_trace_table[currpid][syscall_id].syscall_name = "sys_stacktrace";
			sys_trace_table[currpid][syscall_id].frequency += 1;
			sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
		}
		// end

		return SYSERR;
	}
#endif

	// Code added to trace syscall
	end_time = ctr1000;
	if (is_tracing_syscall) {
		sys_trace_table[currpid][syscall_id].syscall_name = "sys_stacktrace";
		sys_trace_table[currpid][syscall_id].frequency += 1;
		sys_trace_table[currpid][syscall_id].total_time += end_time - start_time;
	}
	// end

	return OK;
}
