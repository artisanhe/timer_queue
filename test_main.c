#include <stdio.h>
#include "time.h"

int main(int argc, char **argv)
{
    timer_queue_thread();
    while (1) {
        sleep(1);
    }
    return 0;
}
