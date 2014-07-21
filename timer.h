#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <unistd.h>
#include "list.h"

#define IS_EQUAL(x, y) ((x) == (y))

typedef struct timer_queue {
	struct list_head node;
	//void (*func) (struct thread *t);
	int (*func) (void);
	struct timeval alarm_time;
	//int create_time;
}timer_queue_t;

#endif
