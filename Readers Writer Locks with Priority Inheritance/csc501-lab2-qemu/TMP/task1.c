#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

#define DEFAULT_LOCK_PRIO 20

#define assert(x,error) if(!(x)){ \
            kprintf(error);\
            return;\
            }

extern int ctr1000;

int mystrncmp(char* des,char* target,int n){
    int i;
    for (i=0;i<n;i++){
        if (target[i] == '.') continue;
        if (des[i] != target[i]) return 1;
    }
    return 0;
}


void lreader2 (char *msg, int lck)
{
    int ret;
    kprintf ("  %s: to acquire lock\n", msg);
    lock (lck, READ, DEFAULT_LOCK_PRIO);
    kprintf ("  %s: acquired lock, sleep 1s\n", msg);
    sleep(2);
    kprintf ("  %s: to release lock\n", msg);
    releaseall (1, lck);
}

void lwriter (char *msg, int lck)
{
    kprintf ("  %s: to acquire lock\n", msg);
    lock (lck, WRITE, DEFAULT_LOCK_PRIO);
    kprintf ("  %s: acquired lock, sleep 2s\n", msg);
    sleep (8);
    kprintf ("  %s: to release lock\n", msg);
    releaseall (1, lck);
}

void lreader1 (char *msg, int lck1, int lck2)
{
    int     ret;

    kprintf ("  %s: to acquire lock\n", msg);
    lock (lck2, READ, DEFAULT_LOCK_PRIO);
    lock (lck1, READ, DEFAULT_LOCK_PRIO);
    
    kprintf ("  %s: acquired lock, sleep 1s\n", msg);
    sleep(2);
    kprintf ("  %s: to release lock\n", msg);
    releaseall (1, lck1, lck2);
}

void locks ()
{
    int lck1, lck2;
    int rd1, rd2;
    int wr1;
    int start, end;
    kprintf("\nTest 1: test the lock implementation\n");
    lck1  = lcreate ();
    lck2  = lcreate ();

    rd1 = create(lreader1, 2000, 50, "reader1", 2, "reader A", lck1, lck2);
    rd2 = create(lreader2, 2000, 55, "reader2", 2, "reader B", lck2);
    wr1 = create(lwriter, 2000, 20, "writer", 2, "writer", lck1);
    start = ctr1000;
    kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
    resume(wr1);
    sleep (1);

    kprintf("-start reader A, then sleep 5s. reader A(prio 50) blocked on the lock\n");
    resume(rd1);
    sleep (5);
    
    kprintf("-start reader B, then sleep 5s. reader B(prio 55) blocked on the lock\n");
    resume (rd2);
	sleep (5);   
    end = ctr1000;
    kprintf("Total time: %d\n", end-start);
    
	kprintf("-kill reader B, then sleep 1s\n");
	
	sleep (1);

	kprintf("-kill reader A, then sleep 1s\n");

    sleep (1);
}

void semreader1 (char *msg, int sem1, int sem2)
{
    int ret;
    kprintf ("  %s: to acquire semaphore\n", msg);
    wait (sem2);
    wait (sem1);
    
    kprintf ("  %s: acquired semaphore, sleep 1s\n", msg);
    sleep(1);
    kprintf ("  %s: to release semaphore\n", msg);
    
    signal(sem2);
    signal(sem1);
    
}

void semreader2 (char *msg, int sem)
{
    int ret;
    kprintf ("  %s: to acquire semaphore\n", msg);
    wait (sem);
    kprintf ("  %s: acquired semaphore, sleep 2s\n", msg);
    sleep(2);
    kprintf ("  %s: to release semaphore\n", msg);
    signal(sem);
}

void semwriter (char *msg, int sem)
{
    kprintf ("  %s: to acquire semaphore\n", msg);
    wait (sem);
    kprintf ("  %s: acquired semaphore, sleep 3s\n", msg);
    sleep (3);
    kprintf ("  %s: to release semaphore\n", msg);
    signal(sem);
}

void semaphores()
{
    int s1, s2;
    int rd1, rd2;
    int wr1;
    int start, end;

    kprintf("\nTest 1: test the semaphore implementation\n");
    s1  = screate (1);
    s2  = screate (1);

    rd1 = create(semreader1, 2000, 50, "reader1", 2, "reader A", s1, s2);
    rd2 = create(semreader2, 2000, 55, "reader2", 2, "reader B", s2);
    wr1 = create(semwriter, 2000, 20, "writer", 2, "writer", s1);
    start = ctr1000;
    kprintf("-start writer, then sleep 1s. semaphore granted to write (prio 20)\n");
    resume(wr1);
    sleep (1);

    kprintf("-start reader A, then sleep 5s. reader A(prio 50) blocked on the semaphore\n");
    resume(rd1);
    sleep (5);

    kprintf("-start reader B, then sleep 5s. reader B(prio 55) blocked on the semaphore\n");
    resume(rd2);
    sleep (5);

    end = ctr1000;
    kprintf("Total time: %d\n", end-start);

	kprintf("-kill reader A, then sleep 1s\n");
	kill (rd1);
    sleep (1);

    kprintf("-kill reader B, then sleep 1s\n");
	kill (rd2);
	
    sleep(1);
}

int main()
{
    semaphores();
    sleep(3);
    locks();
    sleep(2);
    shutdown();
}