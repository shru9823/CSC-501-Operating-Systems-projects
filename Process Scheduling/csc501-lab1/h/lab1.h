#ifndef _LAB1_H_
#define _LAB1_H_

#define AGESCHED 1 
#define LINUXSCHED 2

#include <stdio.h>

extern int schedclass;
void setschedclass(int sched_class);
int getschedclass();

int is_end_of_epoch();
void init_new_epoch();
int get_next_process();

#endif