#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "timer.h"

/* the head of timer queue */
struct list_head g_queue_head;

int flow_process_alarm_sig(int sig);

int print_func()
{
    struct timeval timer_interval = {5, 0};
	printf("Hello word...\n");
    flow_add_timer(print_func, &timer_interval);
}

int flow_add_timer(int (*func_handle) (void), struct timeval *alarm_tm)
{
	timer_queue_t *timer_node = (timer_queue_t *)malloc(sizeof(timer_queue_t));
	if (IS_EQUAL(timer_node, NULL)) {
		perror("add cus-group:");
		return -1;
	}

	memset(timer_node, 0, sizeof(timer_queue_t));
	timer_node->alarm_time.tv_sec = alarm_tm->tv_sec;
	timer_node->alarm_time.tv_usec = alarm_tm->tv_usec;
	timer_node->func = func_handle;
	list_add_tail(&timer_node->node, &g_queue_head);

	return 0;
}

int flow_delete_timer(timer_queue_t *timer)
{
	list_del(&timer->node);
	free(timer);
}

int flow_process_alarm_sig(int sig)
{
	timer_queue_t *pos, *lpos;
	list_for_each_entry_safe(pos, lpos, &g_queue_head, node) {
		if (IS_EQUAL(pos->alarm_time.tv_sec, 0)) {
			pos->func();
			flow_delete_timer(pos);
		}else {
			pos->alarm_time.tv_sec--;
		}
	}
}

int flow_gather_data_run()
{
	sigset_t sig_set;
	int sig = 0;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGALRM);
	alarm(1);
	while (1) {
		if (!IS_EQUAL(sigwait(&sig_set, &sig), 0)) {
			printf("sigmask SIGALRM failed:%s\n", strerror(errno));
			continue;
		}
		flow_process_alarm_sig(sig);
		alarm(1);
	}
}

void *flow_gather_thread() 
{
    struct timeval timer_interval = {10, 0};

	INIT_LIST_HEAD(&g_queue_head);
	flow_add_timer(print_func, &timer_interval);
	flow_gather_data_run();
}

int timer_queue_thread(int argc, char **argv)
{
	int pthread_ret = 0;	
	pthread_t pid;
	sigset_t sig_set;

	/* main thread mask the SIGALRM signal */
	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGALRM);
	if (!IS_EQUAL(pthread_sigmask(SIG_BLOCK, &sig_set, NULL), 0)) {
		printf("sigmask SIGALRM failed:%s\n", strerror(errno));
		exit(0);
	}
	
	pthread_ret = pthread_create(&pid, NULL, flow_gather_thread, NULL);
	if (!IS_EQUAL(pthread_ret, 0)) {
		printf("Can't create pthread %s\n", strerror(pthread_ret));
		exit(0);
	}
    
	return 0;	
}

