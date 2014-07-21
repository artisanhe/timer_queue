#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "timer.h"

/* the head of timer queue */
struct list_head g_queue_head;

void print_func()
{
	printf("Hello word...\n");
}

void process_alarm_sig(int sig)
{
	time_t timer = time(NULL);
	timer_queue_t *pos, *lpos;
	list_for_each_entry_safe(pos, lpos, &g_queue_head, node) {
		if (IS_EQUAL(pos->tm_sec, 0)) {
			pos->func();
			delete_timer(pos);
		}else {
			pos->tm_sec--;
		}
	}
}

int run()
{
	signal(SIGALRM, process_alarm_sig);
	while (1) {
		alarm(1);
		pause();
	}
}

int add_timer(timer_queue_t *node)
{
	list_add_tail(&node->node, &g_queue_head);
	return 0;
}

int delete_timer(timer_queue_t *timer)
{
	list_del(&timer->node);
	return 0;
}

int main(int argc, char **argv)
{
    INIT_LIST_HEAD(&g_queue_head);
	timer_queue_t print_node = {0};
	print_node.func = print_func;
	print_node.tm_sec = 10;
	print_node.create_time = time(NULL);
	add_timer(&print_node);
	run();
	return 0;	
}

