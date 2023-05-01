// /* user.c - main */

// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <stdio.h>

// /*------------------------------------------------------------------------
//  *  main  --  user main program
//  *------------------------------------------------------------------------
//  */
// int main()
// {
// 	kprintf("\n\nHello World, Xinu@QEMU lives\n\n");

//         /* The hook to shutdown QEMU for process-like execution of XINU.
//          * This API call terminates the QEMU process.
//          */
//         shutdown();
// }










// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <lock.h>
// #include <stdio.h>

// #define DEFAULT_LOCK_PRIO 20

// #define assert(x,error) if(!(x)){ \
//             kprintf(error);\
//             return;\
//             }
// int mystrncmp(char* des,char* target,int n){
//     int i;
//     for (i=0;i<n;i++){
//         if (target[i] == '.') continue;
//         if (des[i] != target[i]) return 1;
//     }
//     return 0;

// }
// /*--------------------------------Test 0--------------------------------*/
// void test0 ()
// {
//     int lck;
//     kprintf("\nTest 0: create rwlock.\n");
//     lck  = lcreate ();
//     assert (lck != SYSERR,"Test 0 FAILED\n");
//     kprintf("Test 0 PASSED!\n");
//     ldelete (lck);
// }

// /*--------------------------------Test 1--------------------------------*/

// int testval = 1;
// void reader1 (char *msg, int lck)
// {
//     lock (lck, READ, DEFAULT_LOCK_PRIO);
//     testval = testval*10;
//     sleep (3);
//     testval = testval - 5;
//     releaseall (1, lck);
// }

// void test1 ()
// {
//     int lck;
//     int pid1;
//     int pid2;

//     kprintf("\nTest 1: readers can share the rwlock.\n");
//     testval = 1;
//     lck  = lcreate ();
//     assert (lck != SYSERR,"Test 1 FAILED\n");

//     pid1 = create(reader1, 2000, 20, "reader a", 2, "reader a", lck);
//     pid2 = create(reader1, 2000, 20, "reader b", 2, "reader b", lck);

//     resume(pid1);
// 	sleep(1);
//     resume(pid2);
//     sleep (5);
//     ldelete (lck);
//     kill(pid1);
//     kill(pid2);
//     assert (testval == 90,"Test 1 FAILED\n");
//     kprintf ("Test 1 PASSED!\n");
// }


// /*----------------------------------Test 2---------------------------*/
// void reader2 (char *msg, int lck)
// {
//     lock (lck, READ, DEFAULT_LOCK_PRIO);
//     testval = testval*10;
//     sleep (3);
//     testval = testval-10;
//     releaseall (1, lck);
// }

// void writer2 (char *msg, int lck)
// {
//     lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//     testval = testval*10;
//     sleep (3);
//     testval = testval-5;
//     releaseall (1, lck);
// }

// void test2 ()
// {
//     int     lck;
//     int     pid1;
//     int     pid2;
//     testval = 1;
//     kprintf("\nTest 2: write lock is mutual exclusive\n");
//     lck  = lcreate ();
//     assert (lck != SYSERR,"Test 2 FAILED\n");

//     pid1 = create(writer2, 2000, 20, "writer2", 2, "writer", lck);
//     pid2 = create(reader2, 2000, 20, "reader2", 2, "reader", lck);

//     resume(pid1);
//     sleep (2);
//     resume(pid2);

//     sleep (8);
//     ldelete (lck);
//     kill(pid1);
//     kill(pid2);

//     assert (testval == 40,"Test 2 FAILED\n");
//     kprintf ("Test 2 PASSED!\n");
// }

// /*-----------------------------------Test 3---------------------------*/
// void test3 ()
// {
//     int     lck[NLOCKS];
//     int     last_lck;
//     int     index;

//     kprintf("\nTest 3: return SYSERR if no lock is available\n");
    
//     for (index = 0; index < NLOCKS; index++) {
//         lck[index] = lcreate ();
//         assert (lck[index] != SYSERR,"Test 3 FAILED\n");
//     }

//     last_lck  = lcreate ();
//     assert (last_lck == SYSERR,"Test 3 FAILED\n");

//     for (index = 0; index < NLOCKS; index++) {
//         ldelete (lck[index]);
//     }    
//     kprintf ("Test 3 PASSED!\n");
// }


// /*----------------------------------Test 4---------------------------*/
// void reader4 (char *msg, int lck)
// {
//     int ret;

//     ret = lock (lck, READ, DEFAULT_LOCK_PRIO);
//     assert (ret == DELETED,"Test 4 FAILED\n");
//     kprintf ("Test 4 PASSED!\n");
// }

// void writer4 (char *msg, int lck)
// {
//     lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//     sleep (5);
//     releaseall (1, lck);

// }

// void test4 ()
// {
//     int     lck;
//     int     pid1;
//     int     pid2;

//     kprintf("\nTest 4: return DELETED if lock deleted while waiting\n");
//     lck  = lcreate ();
//     assert (lck != SYSERR,"Test 4 FAILED\n");

//     pid1 = create(writer4, 2000, 20, "writer4", 2, "writer", lck);
//     pid2 = create(reader4, 2000, 20, "reader4", 2, "reader", lck);

//     resume(pid1);
//     sleep (1);

//     resume(pid2);
//     sleep(1);
//     ldelete (lck);

//     sleep (5);
//     kill(pid1);
//     kill(pid2);
// }

// /*-----------------------------------Test 5---------------------------*/
// void test5 ()
// {
//     int     lck[5];
//     int     index;
//     int ret;

//     kprintf("\nTest 5: release multiple locks simultaneously\n");

//     for (index = 0; index < 5; index++) {
//         lck[index] = lcreate ();
//         assert (lck[index] != SYSERR,"Test 5 FAILED\n");

//         ret = lock (lck[index], READ, DEFAULT_LOCK_PRIO);
//         assert (ret == OK,"Test 5 FAILED\n");
//     }

//     ret = releaseall (2, lck[4], lck[0]);
//     assert (ret == OK,"Test 5 FAILED\n");

//     ret = releaseall (3, lck[1], lck[3], lck[2]);
//     assert (ret == OK,"Test 5 FAILED\n");

//     for (index = 0; index < 5; index++) {
//         ldelete (lck[index]);
//     }

//     kprintf ("Test 5 PASSED!\n");
// }

// /*----------------------------------Test 6---------------------------*/
// char output7[10];
// int count7;
// void reader6 (char i, int lck, int lprio)
// {
//     int     ret;

//     //kprintf ("  %c: to acquire lock\n", i);
//     lock (lck, READ, lprio);
//     output7[count7++]=i;
//     //kprintf ("  %c: acquired lock, sleep 3s\n", i);
//     sleep (3);
//     //kprintf ("  %c: to release lock\n", i);
//     output7[count7++]=i;
//     releaseall (1, lck);
    
// }

// void writer6 (char i, int lck, int lprio)
// {
//     //kprintf ("  %c: to acquire lock\n", i);
//     lock (lck, WRITE, lprio);
//     output7[count7++]=i;
//     //kprintf ("  %c: acquired lock, sleep 3s\n", i);
//     sleep (3);
//     //kprintf ("  %c: to release lock\n", i);
//     output7[count7++]=i;
//     releaseall (1, lck);
    
// }

// void test6 ()
// {
//     int     lck;
//     int     rd1, rd2, rd3, rd4;
//     int     wr1;

//     count7 = 0;
//     kprintf("\nTest 6: wait on locks with priority. Expected order of "
//     "lock acquisition is: reader A, reader B, reader C, writer E, reader D\n");
//     lck  = lcreate ();
//     assert (lck != SYSERR,"Test 6 FAILED\n");

//     rd1 = create(reader6, 2000, 20, "reader6", 3, 'A', lck, 20);
//     rd2 = create(reader6, 2000, 20, "reader6", 3, 'B', lck, 30);
//     rd3 = create(reader6, 2000, 20, "reader6", 3, 'C', lck, 40);
//     rd4 = create(reader6, 2000, 20, "reader6", 3, 'D', lck, 20);
//     wr1 = create(writer6, 2000, 20, "writer6", 3, 'E', lck, 25);
    
//     //kprintf("-start reader A, then sleep 1s. lock granted to reader A\n");
//     resume(rd1);
//     sleep (1);

//     //kprintf("-start writer C, then sleep 1s. writer waits for the lock\n");
//     resume(wr1);
//     sleep (1);

//     //kprintf("-start reader B, D, E. reader B is granted lock.\n");
//     resume (rd2);
//     sleep10(1);
//     resume (rd3);
//     sleep10(1);
//     resume (rd4);


//     sleep (10);
//     ldelete (lck);
//     kill(rd1);kill(rd2);kill(rd3);kill(rd4);kill(wr1);
//     kprintf("Output is %s\n",output7);

// }
// /*----------------------------------Test 7---------------------------*/
// int lck1, lck2;
// void procA()
// {
//     kprintf ("rdA try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 20);
//     if(rc == OK) kprintf ("rdA acquires lock 1, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdA releases lock 1\n");
//     releaseall (1, lck1);
// }

// void procB()
// {
//     kprintf ("wrB try to acquire lock 2\n");
//     int rc = lock(lck2, WRITE, 20);
//     if(rc == OK) kprintf ("wrB acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrB releases lock 2\n");
//     releaseall (1, lck2);
// }

// void procC()
// {
//     kprintf ("rdC try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 10);
//     if(rc == OK) kprintf ("rdC acquires lock 1\n");
//     else kprintf("unable to get lock.\n");
//     kprintf ("rdC try to acquire lock 2\n");
//     rc = lock(lck2, READ, 5);
//     if(rc == OK) kprintf ("rdC acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdC releases lock 1&2\n");
//     releaseall (2, lck1, lck2);
// }

// void procD()
// {
//     kprintf ("wrD try to acquire lock 1\n");
//     int rc = lock(lck1, WRITE, 10);
//     if(rc == OK) kprintf ("wrD acquires lock 1\n");
//     else kprintf("unable to get lock.\n");
//     kprintf ("wrD try to acquire lock 2\n");
//     rc = lock(lck2, WRITE, 10);
//     if(rc == OK) kprintf ("wrD acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrD releases lock 1&2\n");
//     releaseall (2, lck1, lck2);
// }

// void procE()
// {
//     kprintf ("rdE try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 5);
//     if(rc == OK) kprintf ("rdE acquires lock 1, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdE releases lock 1\n");
//     releaseall (1, lck1);
// }

// void procF()
// {
//     kprintf ("wrF try to acquire lock 2\n");
//     int rc = lock(lck2, WRITE, 20);
//     if(rc == OK) kprintf ("wrF acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrF releases lock 2\n");
//     releaseall (1, lck2);
// }

// void test7 () 
// {
//     int rdA, rdC, rdE;
//     int wrB, wrD, wrF;

//     kprintf("\nTest 7: \nexpected output:\n rdA try to acquire lock 1\n rdA acquires lock 1, sleep 2s\n wrB try to acquire lock 2\n wrB acquires lock 2, sleep 2s\n rdC try to acquire lock 1\n rdC acquires lock 1\n rdC try to acquire lock 2\n wrD try to acquire lock 1\n rdE try to acquire lock 1\n wrF try to acquire lock 2\n rdA releases lock 1\n wrB releases lock 2\n wrF acquires lock 2, sleep 2s\n wrF releases lock 2\n rdC acquires lock 2, sleep 2s\n rdC releases lock 1&2\n wrD acquires lock 1\n wrD try to acquire lock 2\n wrD acquires lock 2, sleep 2s\n wrD releases lock 1&2\n rdE acquires lock 1, sleep 2s\n rdE releases lock 1\n\n\n real output: \n");

//     kprintf("\nReal Output:\n");
//     lck1  = lcreate();
//     lck2  = lcreate();

//     rdA = create(procA, 2000, 20, "rdA", 1, 1);
//     rdC = create(procC, 2000, 20, "rdC", 1, 1);
//     rdE = create(procE, 2000, 20, "rdE", 1, 1);
//     wrB = create(procB, 2000, 20, "wrB", 1, 1);
//     wrD = create(procD, 2000, 20, "wrD", 1, 1);
//     wrF = create(procF, 2000, 20, "wrF", 1, 1);

//     resume(rdA);
//     resume(wrB);
//     resume(rdC);
//     resume(wrD);
//     resume(rdE);
//     resume(wrF);

//     sleep (10);
//     ldelete (lck1);
//     ldelete (lck2);
//     kill(rdA);kill(rdC);kill(rdE);kill(wrB);kill(wrD);kill(wrF);
// }

// /* -----------------------Test 8 -------------------------------------------------*/
// int lck3;
// void procAA()
// {
//     kprintf ("rdA try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 20);
//     if(rc == OK) kprintf ("rdA acquires lock 1, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdA releases lock 1&3\n");
//     if(releaseall (2, lck1, lck3) == SYSERR) kprintf("procA doesn't hold this lock\n");
// }

// void procBB()
// {
//     kprintf ("wrB try to acquire lock 2\n");
//     int rc = lock(lck2, WRITE, 20);
//     if(rc == OK) kprintf ("wrB acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrB releases lock 2\n");
//     releaseall (1, lck2);
// }

// void procCC()
// {
//     kprintf ("rdC try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 10);
//     if(rc == OK) kprintf ("rdC acquires lock 1\n");
//     else kprintf("unable to get lock.\n");
//     kprintf ("rdC try to acquire lock 2\n");
//     rc = lock(lck2, READ, 5);
//     if(rc == OK) kprintf ("rdC acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdC releases lock 1&2\n");
//     releaseall (2, lck1, lck2);
// }

// void procDD()
// {
//     kprintf ("wrD try to acquire lock 1\n");
//     int rc = lock(lck1, WRITE, 10);
//     if(rc == OK) kprintf ("wrD acquires lock 1\n");
//     else kprintf("unable to get lock.\n");
//     kprintf ("wrD try to acquire lock 2\n");
//     rc = lock(lck2, WRITE, 10);
//     if(rc == OK) kprintf ("wrD acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrD releases lock 1&2\n");
//     releaseall (2, lck1, lck2);
// }

// void procEE()
// {
//     kprintf ("rdE try to acquire lock 1\n");
//     int rc = lock(lck1, READ, 5);
//     if(rc == OK) kprintf ("rdE acquires lock 1, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("rdE releases lock 1\n");
//     releaseall (1, lck1);
// }

// void procFF()
// {
//     kprintf ("wrF try to acquire lock 2\n");
//     int rc = lock(lck2, WRITE, 20);
//     if(rc == OK) kprintf ("wrF acquires lock 2, sleep 2s\n");
//     else kprintf("unable to get lock.\n");
//     sleep (2);
//     kprintf ("wrF releases lock 2\n");
//     releaseall (1, lck2);
// }

// void test8 () 
// {
//     int rdA, rdC, rdE;
//     int wrB, wrD, wrF;

//     kprintf("\nTest 8: \nexpected output:\n rdA try to acquire lock 1\n rdA acquires lock 1, sleep 2s\n wrB try to acquire lock 2\n wrB acquires lock 2, sleep 2s\n rdC try to acquire lock 1\n rdC acquires lock 1\n rdC try to acquire lock 2\n wrD try to acquire lock 1\n rdE try to acquire lock 1\n wrF try to acquire lock 2\n rdA releases lock 1&3\n procA doesn't hold this lock\n wrB releases lock 2\n wrF acquires lock 2, sleep 2s\n wrF releases lock 2\n rdC acquires lock 2, sleep 2s\n rdC releases lock 1&2\n wrD acquires lock 1\n wrD try to acquire lock 2\n wrD acquires lock 2, sleep 2s\n wrD releases lock 1&2\n rdE acquires lock 1, sleep 2s\n rdE releases lock 1\n\n\n real output: \n");

//     kprintf("\nReal Output:\n");
//     lck1  = lcreate();
//     lck2  = lcreate();
//     lck3  = lcreate();

//     rdA = create(procAA, 2000, 20, "rdA", 1, 1);
//     rdC = create(procCC, 2000, 20, "rdC", 1, 1);
//     rdE = create(procEE, 2000, 20, "rdE", 1, 1);
//     wrB = create(procBB, 2000, 20, "wrB", 1, 1);
//     wrD = create(procDD, 2000, 20, "wrD", 1, 1);
//     wrF = create(procFF, 2000, 20, "wrF", 1, 1);

//     resume(rdA);
//     resume(wrB);
//     resume(rdC);
//     resume(wrD);
//     resume(rdE);
//     resume(wrF);
//     sleep (10);
//     ldelete (lck1); ldelete(lck2); ldelete(lck3);
//     kill(rdA);kill(rdC);kill(rdE);kill(wrB);kill(wrD);kill(wrF);
//     sleep(1);
// }
// /*-------------------------Test 9 ----------------------------------------------------------*/
// void reader(char *msg, int lck, int lprio) {
//     int rc;
//     rc = lock(lck, READ, lprio);
//     if (rc == SYSERR) {
//     kprintf ("  %s: lock returned SYSERR\n", msg);
//         return;
//     }
//     if (rc == DELETED) {
//         kprintf ("  %s: lock was DELETED\n", msg);
//         return;
//     }
//     kprintf ("  %s: acquired lock, sleep 3s\n", msg);
//     sleep (3);
//     releaseall (1, lck);
// }

// void writer(char *msg, int lck, int lprio) {
//     int rc;
//     rc = lock(lck, WRITE, lprio);
//     if (rc == SYSERR) {
//         kprintf ("  %s: lock returned SYSERR\n", msg);
//         return;
//     }
//     if (rc == DELETED) {
//         kprintf ("  %s: lock was DELETED\n", msg);
//         return;
//     }
//     kprintf ("  %s: acquired lock, sleep 3s\n", msg);
//     sleep (3);
//     releaseall (1, lck);
// }
// void test9 ()
// {
//     int i;
//     int lastlck;
//     int rd1, rd2, rd3, rd4;
//     int wr1;

//     kprintf("\nTest 9:\nexpected output:\nreader A: acquired lock, sleep 3s\nwriter C: lock was DELETED\nreader B: lock returned SYSERR\nreader D: acquired lock, sleep 3s\n");

//     kprintf("\nReal Output:\n");
//     // Create original lock
//     lastlck = lcreate();

//     // Create processes that use original lock
//     rd1 = create(reader, 2000, 20, "reader", 3, "reader A", lastlck, 20);
//     rd2 = create(reader, 2000, 20, "reader", 3, "reader B", lastlck, 20);
//     wr1 = create(writer, 2000, 20, "writer", 3, "writer C", lastlck, 30);

//     // Start reader A
//     resume(rd1);
//     sleep(1);

//     // Start writer C. Will get returned DELETED after lock gets
//     // deleted.
//     resume(wr1);
//     sleep(1);

//     // Delete lock
//     ldelete(lastlck);

//     for (i=1; i<NLOCKS; i++) {
//        lastlck = lcreate();
//        ldelete(lastlck);
//     }

//     // Create a new lock (this lock will use same slot as original)
//     lastlck = lcreate();

//     // Create a new process to use the new lock spot. 
//     rd3 = create(reader, 2000, 20, "reader", 3, "reader D", lastlck, 20);

//     // Start reader B: Should get returned SYSERR since the lock it
//     // uses was already deleted and a new lock is using that index
//     // kprintf("start\n");
//     resume(rd2);

//     // Start reader D that uses new lock
//     resume(rd3);
//     sleep(4);
// }

// int main( )
// {
//     test0();
//     test1();
//     test2();
//     test3();
//     test4();
//     test5();
//     test6();
//     test7();
//     test8();
//     test9();
//     // shutdown();
// }

























// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <lock.h>
// #include <stdio.h>

// #define assert(x,error) if(!(x)){ \
//             kprintf(error);\
//             return;\
//             }

// #define DEFAULT_LOCK_PRIO 20

// int mystrncmp(char* des,char* target,int n){
//     int i;
//     for (i=0;i<n;i++){
//         if (target[i] == '.') continue;
//         if (des[i] != target[i]) return 1;
//     }
//     return 0;
// }

// /*--------------------------------Test 1--------------------------------*/
 
// void reader1 (char *msg, int lck)
// {
// 	lock (lck, READ, DEFAULT_LOCK_PRIO);
// 	kprintf ("  %s: acquired lock, sleep 2s\n", msg);
// 	sleep (2);
// 	kprintf ("  %s: to release lock\n", msg);
// 	releaseall (1, lck);
// }

// void test1 ()
// {
// 	int	lck;
// 	int	pid1;
// 	int	pid2;

// 	kprintf("\nTest 1: readers can share the rwlock\n");
// 	lck  = lcreate ();
// 	assert (lck != SYSERR, "Test 1 failed");

// 	pid1 = create(reader1, 2000, 20, "reader a", 2, "reader a", lck);
// 	pid2 = create(reader1, 2000, 20, "reader b", 2, "reader b", lck);

// 	resume(pid1);
// 	resume(pid2);
	
// 	sleep (5);
// 	ldelete (lck);
// 	kprintf ("Test 1 ok\n");
// }

// /*----------------------------------Test 2---------------------------*/
// char output2[10];
// int count2;
// void reader2 (char msg, int lck, int lprio)
// {
//         int     ret;

//         kprintf ("  %c: to acquire lock\n", msg);
//         lock (lck, READ, lprio);
//         output2[count2++]=msg;
//         kprintf ("  %c: acquired lock, sleep 3s\n", msg);
//         sleep (3);
//         output2[count2++]=msg;
//         kprintf ("  %c: to release lock\n", msg);
// 	releaseall (1, lck);
// }

// void writer2 (char msg, int lck, int lprio)
// {
// 	kprintf ("  %c: to acquire lock\n", msg);
//         lock (lck, WRITE, lprio);
//         output2[count2++]=msg;
//         kprintf ("  %c: acquired lock, sleep 3s\n", msg);
//         sleep (3);
//         output2[count2++]=msg;
//         kprintf ("  %c: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void test2 ()
// {
//         count2 = 0;
//         int     lck;
//         int     rd1, rd2, rd3, rd4;
//         int     wr1;

//         kprintf("\nTest 2: wait on locks with priority. Expected order of"
// 		" lock acquisition is: reader A, reader B, reader D, writer C & reader E\n");
//         lck  = lcreate ();
//         assert (lck != SYSERR, "Test 2 failed");

// 	rd1 = create(reader2, 2000, 20, "reader2", 3, 'A', lck, 20);
// 	rd2 = create(reader2, 2000, 20, "reader2", 3, 'B', lck, 30);
// 	rd3 = create(reader2, 2000, 20, "reader2", 3, 'D', lck, 25);
// 	rd4 = create(reader2, 2000, 20, "reader2", 3, 'E', lck, 20);
//         wr1 = create(writer2, 2000, 20, "writer2", 3, 'C', lck, 28);
	
//         kprintf("-start reader A, then sleep 1s. lock granted to reader A\n");
//         resume(rd1);
//         sleep (1);

//         kprintf("-start writer C, then sleep 1s. writer waits for the lock\n");
//         resume(wr1);
//         sleep10 (1);


//         kprintf("-start reader B, D, E. reader B is granted lock.\n");
//         resume (rd2);
// 	resume (rd3);
// 	resume (rd4);


//         sleep (15);
//         kprintf("output=%s\n", output2);
//         assert(mystrncmp(output2,"ABABCCDEED",10)==0,"Test 2 FAILED\n");
//         kprintf ("Test 2 OK\n");
// }

// /*----------------------------------Test 3---------------------------*/
// void reader3 (char *msg, int lck)
// {
//         int     ret;

//         kprintf ("  %s: to acquire lock\n", msg);
//         lock (lck, READ, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock\n", msg);
//         kprintf ("  %s: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void writer3 (char *msg, int lck)
// {
//         kprintf ("  %s: to acquire lock\n", msg);
//         lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock, sleep 10s\n", msg);
//         sleep (10);
//         kprintf ("  %s: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void test3 ()
// {
//         int     lck;
//         int     rd1, rd2;
//         int     wr1;

//         kprintf("\nTest 3: test the basic priority inheritence\n");
//         lck  = lcreate ();
//         assert (lck != SYSERR, "Test 3 failed");

//         rd1 = create(reader3, 2000, 25, "reader3", 2, "reader A", lck);
//         rd2 = create(reader3, 2000, 30, "reader3", 2, "reader B", lck);
//         wr1 = create(writer3, 2000, 20, "writer3", 2, "writer", lck);

//         kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
//         resume(wr1);
//         sleep (1);

//         kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
//         resume(rd1);
//         sleep (1);
// 	assert (getprio(wr1) == 25, "Test 3 failed");

//         kprintf("-start reader B, then sleep 1s. reader B(prio 30) blocked on the lock\n");
//         resume (rd2);
// 	sleep (1);
// 	assert (getprio(wr1) == 30, "Test 3 failed");
	
// 	kprintf("-kill reader B, then sleep 1s\n");
// 	kill (rd2);
// 	sleep (1);
// 	assert (getprio(wr1) == 25, "Test 3 failed");

// 	kprintf("-kill reader A, then sleep 1s\n");
// 	kill (rd1);
// 	sleep(1);
// 	assert(getprio(wr1) == 20, "Test 3 failed");

//         sleep (8);
//         kprintf ("Test 3 OK\n");
// }

// /*----------------------------------Test 4---------------------------*/
// // void reader4(char *msg, int lck1, int lck2)
// // {
// //         int     ret;
// //         kprintf ("  %s: to acquire lock1\n", msg);
// //         lock (lck1, READ, DEFAULT_LOCK_PRIO);
// //         kprintf ("  %s: acquired lock1\n", msg);
// //         kprintf ("  %s: to release lock1\n", msg);
// //         kprintf ("  %s: to release lock2 before acquisition\n", msg);
// //         ret = releaseall(2, lck1, lck2);
// //         kprintf("  releaseall(2, lck1, lck2) returned %d\n", ret);
// //         assert(ret == SYSERR, "Test 4 failed (incorrect return value)");
// //         assert(proctab[currpid].llock[lck1] == FALSE, "Test 4 failed (acquired lock1 is not released properly)");
// // }

// // void test4()
// // {
// //         int lck1, lck2;
// //         int rd1;

// //         kprintf("\nTest 4: lock release without acquisition\n");
// //         lck1 = lcreate();
// //         lck2 = lcreate();
// //         assert (lck1 != SYSERR, "Test 4 failed (lck1)");
// //         assert (lck2 != SYSERR, "Test 4 failed (lck2)");
// //         rd1 = create(reader4, 2000, 25, "reader4", 2, "reader A", lck1, lck2);
// //         resume(rd1);
// //         kprintf ("Test 4 OK\n");
// // }

// /*----------------------------------Test 5---------------------------*/
// char output5[6];
// int count5;
// void reader5 (char msg, int lck, int lprio)
// {
//         int     ret;

//         kprintf ("  %c: to acquire lock\n", msg);
//         lock (lck, READ, lprio);
//         output5[count5++]=msg;
//         kprintf ("  %c: acquired lock, sleep 3s\n", msg);
//         sleep (3);
//         output5[count5++]=msg;
//         kprintf ("  %c: to release lock\n", msg);
// 	releaseall (1, lck);
// }

// void writer5 (char msg, int lck, int lprio)
// {
// 	kprintf ("  %c: to acquire lock\n", msg);
//         lock (lck, WRITE, lprio);
//         output5[count5++]=msg;
//         kprintf ("  %c: acquired lock, sleep 3s\n", msg);
//         sleep (3);
//         output5[count5++]=msg;
//         kprintf ("  %c: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void test5 ()
// {
//         count5 = 0;
//         int     lck;
//         int     rd1;
//         int     wr1, wr2;

//         kprintf("\nTest 5: wait on locks with writer priority equal to reader priority. Expected order of"
// 		" lock acquisition is: writer B, writer C & reader A\n");
//         lck  = lcreate ();
//         assert (lck != SYSERR, "Test 5 failed");
	
// 	wr1 = create(writer5, 2000, 20, "writer5", 3, 'B', lck, 30);
//         rd1 = create(reader5, 2000, 20, "reader5", 3, 'A', lck, 20);
//         wr2 = create(writer5, 2000, 20, "writer5", 3, 'C', lck, 20);
	
//         kprintf("-start writer B, then sleep 1s. lock granted to writer B\n");
//         resume(wr1);
//         sleep (1);

//         kprintf("-start reader A, then sleep 0.1s. reader waits for the lock\n");
//         resume(rd1);
//         sleep (0.1);

//         kprintf("-start writer C, then sleep 1s. writer waits for the lock\n");
//         resume(wr2);
//         sleep(1);

//         sleep (15);
//         kprintf("output=%s\n", output5);
//         assert(mystrncmp(output5,"BBCCAA",6)==0,"Test 5 FAILED\n");
//         kprintf ("Test 5 OK\n");
// }

// /*----------------------------------Test 6---------------------------*/
// void reader6 (char *msg, int lck)
// {
//         int     ret;

//         kprintf ("  %s: to acquire lock\n", msg);
//         lock (lck, READ, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock\n", msg);
//         kprintf ("  %s: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void writer6 (char *msg, int lck)
// {
//         kprintf ("  %s: to acquire lock\n", msg);
//         lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock, sleep 10s\n", msg);
//         sleep (10);
//         kprintf ("  %s: to release lock\n", msg);
//         releaseall (1, lck);
// }

// void test6 ()
// {
//         int     lck;
//         int     rd1, rd2;
//         int     wr1;

//         kprintf("\nTest 6: test priority inheritence with chprio \n");
//         lck  = lcreate ();
//         assert (lck != SYSERR, "Test 6 failed");

//         rd1 = create(reader3, 2000, 25, "reader3", 2, "reader A", lck);
//         rd2 = create(reader3, 2000, 30, "reader3", 2, "reader B", lck);
//         wr1 = create(writer3, 2000, 20, "writer3", 2, "writer", lck);

//         kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
//         resume(wr1);
//         sleep (1);

//         kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
//         resume(rd1);
//         sleep (1);
// 	assert (getprio(wr1) == 25, "Test 6 failed");

//         kprintf("-increase reader A priority (prio 35), then sleep 1s.\n");
//         chprio(rd1, 35);
//         sleep(1);
//         assert (getprio(wr1) == 35, "Test 6 failed");

//         kprintf("-start reader B, then sleep 1s. reader B(prio 30) blocked on the lock\n");
//         resume (rd2);
// 	sleep (1);
//         kprintf("prio B %d", getprio(wr1));
// 	assert (getprio(wr1) == 35, "Test 6 failed");
	
// 	kprintf("-kill reader A, then sleep 1s. writer should inherit priority of reader B (prio 30)\n");
// 	kill (rd1);
// 	sleep (1);
// 	assert (getprio(wr1) == 30, "Test 6 failed");

// 	kprintf("-kill reader B, then sleep 1s\n");
// 	kill (rd2);
// 	sleep(1);
// 	assert(getprio(wr1) == 20, "Test 6 failed");

//         sleep (15);
//         kprintf ("Test 6 OK\n");
// }

// /*----------------------------------Test 7---------------------------*/
// void T7A (char *msg, int lck1, int lck2)
// {
//         int     ret;
//         kprintf ("  %s: to acquire lock1\n", msg);
//         lock (lck1, READ, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock1\n", msg);
//         sleep(3);
//         kprintf ("  %s: to acquire lock2\n", msg);
//         lock (lck2, WRITE, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock2\n", msg);

//         kprintf("  %s: to release lock1 and lock2\n", msg);
//         releaseall (2, lck1, lck2);
// }

// void T7B (char *msg, int lck)
// {
//         int     ret;
//         kprintf ("  %s: to acquire lock2\n", msg);
//         lock (lck, READ, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock2\n", msg);
//         sleep(10);
//         kprintf("  %s: to release lock2\n", msg);
//         releaseall (1, lck);
// }

// void T7C (char *msg, int lck)
// {
//         kprintf ("  %s: to acquire lock1\n", msg);
//         lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//         kprintf ("  %s: acquired lock1, sleep 10s\n", msg);
//         sleep (10);
//         kprintf ("  %s: to release lock1\n", msg);
//         releaseall (1, lck);
// }

// void test7 ()
// {
//         int     lck1, lck2;
//         int     wr1, wr2, wr3;

//         kprintf("\nTest 7: test transitive priority inheritance\n");
//         lck1  = lcreate ();
//         lck2  = lcreate ();
//         assert (lck1 != SYSERR, "Test 7 failed (lck1)");
//         assert (lck2 != SYSERR, "Test 7 failed (lck2)");

//         wr1 = create(T7A, 2000, 10, "7A", 3, "A", lck1, lck2);
//         wr2 = create(T7B, 2000, 20, "7B", 2, "B", lck2);
//         wr3 = create(T7C, 2000, 30, "7C", 2, "C", lck1);

//         kprintf("-start A, then sleep 1s. lock1 granted to A (prio 10)\n");
//         resume(wr1);
//         sleep (1);

//         kprintf("-start B, then sleep 3s. lock2 granted to B (prio 20)\n");
//         resume(wr2);
//         sleep (3);

//         kprintf("-start C, then sleep 1s. C(prio 30) blocked on the lock1\n");
//         resume (wr3);
// 	sleep (1);
// 	assert (getprio(wr1) == 30, "Test 7 failed (wr1)");
//         assert (getprio(wr2) == 30, "Test 7 failed (wr2)");

//         sleep (10);
//         kprintf ("Test 7 OK\n");
// }

// int main( )
// {
//         /* These test cases are only used for test purposes.
//          * The provided results do not guarantee your correctness.
//          * You need to read the PA2 instruction carefully.
//          */
// 	test1();
// 	test2();
// 	test3();
//         // test4();
//         test5();
//         test6();
//         test7();

//         /* The hook to shutdown QEMU for process-like execution of XINU.
//          * This API call exists the QEMU process.
//          */
//         shutdown();
// }


























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
int mystrncmp(char* des,char* target,int n){
    int i;
    for (i=0;i<n;i++){
        if (target[i] == '.') continue;
        if (des[i] != target[i]) return 1;
    }
    return 0;
}

/*--------------------------------Test 1--------------------------------*/
 
void reader1 (char *msg, int lck)
{
	lock (lck, READ, DEFAULT_LOCK_PRIO);
	kprintf ("  %s: acquired lock, sleep 2s\n", msg);
	sleep (2);
	kprintf ("  %s: to release lock\n", msg);
	releaseall (1, lck);
}

void test1 ()
{
	int	lck;
	int	pid1;
	int	pid2;

	kprintf("\nTest 1: readers can share the rwlock\n");
	lck  = lcreate ();
	assert (lck != SYSERR, "Test 1 failed");

	pid1 = create(reader1, 2000, 20, "reader a", 2, "reader a", lck);
	pid2 = create(reader1, 2000, 20, "reader b", 2, "reader b", lck);

	resume(pid1);
	resume(pid2);
	
	sleep (5);
	ldelete (lck);
	kprintf ("Test 1 ok\n");
}

/*----------------------------------Test 2---------------------------*/
char output2[10];
int count2;
void reader2 (char msg, int lck, int lprio)
{
        int     ret;

        kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, READ, lprio);
        output2[count2++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
        sleep (3);
        output2[count2++]=msg;
        kprintf ("  %c: to release lock\n", msg);
	releaseall (1, lck);
}

void writer2 (char msg, int lck, int lprio)
{
	kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, WRITE, lprio);
        output2[count2++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
        sleep (3);
        output2[count2++]=msg;
        kprintf ("  %c: to release lock\n", msg);
        releaseall (1, lck);
}

void test2 ()
{
        count2 = 0;
        int     lck;
        int     rd1, rd2, rd3, rd4;
        int     wr1;

        kprintf("\nTest 2: wait on locks with priority. Expected order of"
		" lock acquisition is: reader A, reader B, reader D, writer C & reader E\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 2 failed");

	rd1 = create(reader2, 2000, 20, "reader2", 3, 'A', lck, 20);
	rd2 = create(reader2, 2000, 20, "reader2", 3, 'B', lck, 30);
	rd3 = create(reader2, 2000, 20, "reader2", 3, 'D', lck, 25);
	rd4 = create(reader2, 2000, 20, "reader2", 3, 'E', lck, 20);
        wr1 = create(writer2, 2000, 20, "writer2", 3, 'C', lck, 28);
	
        kprintf("-start reader A, then sleep 1s. lock granted to reader A\n");
        resume(rd1);
        sleep (1);

        kprintf("-start writer C, then sleep 1s. writer waits for the lock\n");
        resume(wr1);
        sleep10 (1);


        kprintf("-start reader B, D, E. reader B is granted lock.\n");
        resume (rd2);
	resume (rd3);
	resume (rd4);


        sleep (15);
        kprintf("output=%s\n", output2);
        assert(mystrncmp(output2,"ABABCCDEED",10)==0,"Test 2 FAILED\n");
        kprintf ("Test 2 OK\n");
}

/*----------------------------------Test 3---------------------------*/
void reader3 (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void writer3 (char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void test3 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(reader3, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(reader3, 2000, 30, "reader3", 2, "reader B", lck);
        wr1 = create(writer3, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);
	assert (getprio(wr1) == 25, "Test 3 failed");

        kprintf("-start reader B, then sleep 1s. reader B(prio 30) blocked on the lock\n");
        resume (rd2);
	sleep (1);
	assert (getprio(wr1) == 30, "Test 3 failed");
	
	kprintf("-kill reader B, then sleep 1s\n");
	kill (rd2);
	sleep (1);
	assert (getprio(wr1) == 25, "Test 3 failed");

	kprintf("-kill reader A, then sleep 1s\n");
	kill (rd1);
	sleep(1);
	assert(getprio(wr1) == 20, "Test 3 failed");

        sleep (8);
        kprintf ("Test 3 OK\n");
}

int main( )
{
        /* These test cases are only used for test purposes.
         * The provided results do not guarantee your correctness.
         * You need to read the PA2 instruction carefully.
         */
        kprintf ("Test 3 OK\n");
	test1();
	test2();
	test3();

        /* The hook to shutdown QEMU for process-like execution of XINU.
         * This API call exists the QEMU process.
         */
        shutdown();
}



// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <lock.h>
// #include <stdio.h>

// #define DEFAULT_LOCK_PRIO 20

// #define assert(x,error) if(!(x)){ \
//             kprintf(error);\
//             return;\
//             }

// extern int ctr1000;

// int mystrncmp(char* des,char* target,int n){
//     int i;
//     for (i=0;i<n;i++){
//         if (target[i] == '.') continue;
//         if (des[i] != target[i]) return 1;
//     }
//     return 0;
// }


// void lreader2 (char *msg, int lck)
// {
//     int ret;
//     kprintf ("  %s: to acquire lock\n", msg);
//     lock (lck, READ, DEFAULT_LOCK_PRIO);
//     kprintf ("  %s: acquired lock, sleep 1s\n", msg);
//     sleep(2);
//     kprintf ("  %s: to release lock\n", msg);
//     releaseall (1, lck);
// }

// void lwriter (char *msg, int lck)
// {
//     kprintf ("  %s: to acquire lock\n", msg);
//     lock (lck, WRITE, DEFAULT_LOCK_PRIO);
//     kprintf ("  %s: acquired lock, sleep 2s\n", msg);
//     sleep (8);
//     kprintf ("  %s: to release lock\n", msg);
//     releaseall (1, lck);
// }

// void lreader1 (char *msg, int lck1, int lck2)
// {
//     int     ret;

//     kprintf ("  %s: to acquire lock\n", msg);
//     lock (lck2, READ, DEFAULT_LOCK_PRIO);
//     lock (lck1, READ, DEFAULT_LOCK_PRIO);
    
//     kprintf ("  %s: acquired lock, sleep 1s\n", msg);
//     sleep(2);
//     kprintf ("  %s: to release lock\n", msg);
//     releaseall (1, lck1, lck2);
// }

// void locks ()
// {
//     int lck1, lck2;
//     int rd1, rd2;
//     int wr1;
//     int start, end;
//     kprintf("\nTest 1: test the lock implementation\n");
//     lck1  = lcreate ();
//     lck2  = lcreate ();

//     rd1 = create(lreader1, 2000, 50, "reader1", 2, "reader A", lck1, lck2);
//     rd2 = create(lreader2, 2000, 55, "reader2", 2, "reader B", lck2);
//     wr1 = create(lwriter, 2000, 20, "writer", 2, "writer", lck1);
//     start = ctr1000;
//     kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
//     resume(wr1);
//     sleep (1);

//     kprintf("-start reader A, then sleep 5s. reader A(prio 50) blocked on the lock\n");
//     resume(rd1);
//     sleep (5);
    
//     kprintf("-start reader B, then sleep 5s. reader B(prio 55) blocked on the lock\n");
//     resume (rd2);
// 	sleep (5);   
//     end = ctr1000;
//     kprintf("Total time: %d\n", end-start);
    
// 	kprintf("-kill reader B, then sleep 1s\n");
	
// 	sleep (1);

// 	kprintf("-kill reader A, then sleep 1s\n");

//     sleep (1);
// }

// void semreader1 (char *msg, int sem1, int sem2)
// {
//     int ret;
//     kprintf ("  %s: to acquire semaphore\n", msg);
//     wait (sem2);
//     wait (sem1);
    
//     kprintf ("  %s: acquired semaphore, sleep 1s\n", msg);
//     sleep(1);
//     kprintf ("  %s: to release semaphore\n", msg);
    
//     signal(sem2);
//     signal(sem1);
    
// }

// void semreader2 (char *msg, int sem)
// {
//     int ret;
//     kprintf ("  %s: to acquire semaphore\n", msg);
//     wait (sem);
//     kprintf ("  %s: acquired semaphore, sleep 2s\n", msg);
//     sleep(2);
//     kprintf ("  %s: to release semaphore\n", msg);
//     signal(sem);
// }

// void semwriter (char *msg, int sem)
// {
//     kprintf ("  %s: to acquire semaphore\n", msg);
//     wait (sem);
//     kprintf ("  %s: acquired semaphore, sleep 3s\n", msg);
//     sleep (3);
//     kprintf ("  %s: to release semaphore\n", msg);
//     signal(sem);
// }

// void semaphores()
// {
//     int s1, s2;
//     int rd1, rd2;
//     int wr1;
//     int start, end;

//     kprintf("\nTest 1: test the semaphore implementation\n");
//     s1  = screate (1);
//     s2  = screate (1);

//     rd1 = create(semreader1, 2000, 50, "reader1", 2, "reader A", s1, s2);
//     rd2 = create(semreader2, 2000, 55, "reader2", 2, "reader B", s2);
//     wr1 = create(semwriter, 2000, 20, "writer", 2, "writer", s1);
//     start = ctr1000;
//     kprintf("-start writer, then sleep 1s. semaphore granted to write (prio 20)\n");
//     resume(wr1);
//     sleep (1);

//     kprintf("-start reader A, then sleep 5s. reader A(prio 50) blocked on the semaphore\n");
//     resume(rd1);
//     sleep (5);

//     kprintf("-start reader B, then sleep 5s. reader B(prio 55) blocked on the semaphore\n");
//     resume(rd2);
//     sleep (5);

//     end = ctr1000;
//     kprintf("Total time: %d\n", end-start);

// 	kprintf("-kill reader A, then sleep 1s\n");
// 	kill (rd1);
//     sleep (1);

//     kprintf("-kill reader B, then sleep 1s\n");
// 	kill (rd2);
	
//     sleep(1);
// }

// int main()
// {
//     semaphores();
//     sleep(3);
//     locks();
//     sleep(2);
//     shutdown();
// }