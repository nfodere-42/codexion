#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "codexion.h"
#include "heap.h"

typedef struct s_scheduler
{
    t_scheduler_type    type;
    t_heap              queue;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
}   t_scheduler;

void    scheduler_init(t_scheduler *s, t_scheduler_type type);
void    scheduler_destroy(t_scheduler *s);

void    scheduler_request(t_scheduler *s, t_coder *c);
t_coder *scheduler_pop(t_scheduler *s);

int     compare_priorities(long a, long b);
void    scheduler_print_queue(t_scheduler *s);

#endif
