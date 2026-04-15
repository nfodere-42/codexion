#include "../include/scheduler.h"
#include "../include/heap.h"

int compare_priorities(long a, long b)
{
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

void scheduler_print_queue(t_scheduler *s)
{
    pthread_mutex_lock(&s->mutex);
    for (int i = 0; i < s->queue.size; i++)
    {
        printf("Node %d: coder %d, priority %ld\n",
            i,
            s->queue.nodes[i].coder->id,
            s->queue.nodes[i].priority);
    }
    pthread_mutex_unlock(&s->mutex);
}
