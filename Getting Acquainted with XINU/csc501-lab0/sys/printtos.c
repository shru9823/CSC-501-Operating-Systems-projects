#include <lab0.h>

unsigned long *ebp;
unsigned long *esp;

void printtos(){
	kprintf("\nTask 3: void printtos()\n");

	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	kprintf("Before [0x%08x]:0x%08x\n", ebp+2,*(ebp+2));
	kprintf("After  [0x%08x]: 0x%08x \n",ebp, *ebp);
	kprintf("\telement[0x%08x]: 0x%08x\n\telement[0x%08x]: 0x%08x\n\telement[0x%08x]: 0x%08x\n\telement[0x%08x]: 0x%08x\n", esp+1, *(esp+1), esp+2, *(esp+2), esp+3, *(esp+3), esp+4, *(esp+4));
}