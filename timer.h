#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <unistd.h>
#include "list.h"

#define IS_EQUAL(x, y) ((x) == (y))

typedef struct timer_queue {
	struct list_head node;
	void (*func) (void);
	int tm_sec;
	int create_time;
}timer_queue_t;

#endif
