/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
unsigned long *esp;
int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");

	//Task 1 function call
	long num = 0xaabbccdd, ans;
	kprintf("\n\nHey World, Xinu lives\n\n");
	ans = zfunction(num);
	kprintf("\nTask 1: zfunction\n");
	kprintf("\tzfunction (0x%x): 0x%x\n", num, ans);

	//Task 2 function call
	printsegaddress();

	//Task 3 function call
	printtos();

	//Task 4 function call
	printprocstks(5);

	//Task 5 function call
	// Test case 1
	syscallsummary_start();
	getpid();
	sleep(1);
	sleep(1);
	sleep(1);
	getpid();
	sleep(1);
	syscallsummary_stop();
	printsyscallsummary();

	// Test case 2
	syscallsummary_start();
	getpid();
	sleep(1);
	sleep(1);
	sleep(1);
	sleep(1);
	getpid();
	sleep(1);
	syscallsummary_stop();
	printsyscallsummary();

	// Test case 3
	getpid();
	sleep(1);
	sleep(1);
	sleep(1);
	sleep(1);
	getpid();
	sleep(1);
	printsyscallsummary();

	return 0;
}
