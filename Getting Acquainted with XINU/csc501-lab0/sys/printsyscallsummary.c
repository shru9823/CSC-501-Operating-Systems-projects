#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

int is_tracing_syscall = 0;
unsigned long ctr1000;
struct sys_trace sys_trace_table[50][27]; 


void syscallsummary_start(){
    is_tracing_syscall = 1;
}

void syscallsummary_stop(){
    is_tracing_syscall = 0;
}

void printsyscallsummary(){
    kprintf("\nTask 5: void printsyscallsummary()\n");
    int pid = 0;
    for(pid = 0; pid < NPROC; pid++){
        Bool this_process = FALSE;
        int syscall_id = 0;
        for(syscall_id = 0; syscall_id < 27; syscall_id++){
            if(sys_trace_table[pid][syscall_id].frequency > 0){
                if(!this_process){
                    this_process = TRUE;
                    kprintf("Process [pid:%d]\n", pid);
                }
                unsigned long avg_time = 0;
                avg_time = sys_trace_table[pid][syscall_id].total_time / sys_trace_table[pid][syscall_id].frequency;
                kprintf("\tSyscall: %s, count: %d, average execution time: %lu(ms)\n", sys_trace_table[pid][syscall_id].syscall_name, sys_trace_table[pid][syscall_id].frequency, avg_time);
                sys_trace_table[pid][syscall_id].syscall_name = "";
                sys_trace_table[pid][syscall_id].frequency = 0;
                sys_trace_table[pid][syscall_id].total_time = 0.0;

            }
        }

    }
}