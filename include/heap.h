#ifndef HEAP_H
#define HEAP_H

#include "codexion.h"

typedef struct s_heap_node
{
    long        priority;
    t_coder     *coder;
}   t_heap_node;

typedef struct s_heap
{
    t_heap_node *nodes;
    int         size;
    int         capacity;
}   t_heap;

void        heap_init(t_heap *h);
void        heap_destroy(t_heap *h);
void        heap_push(t_heap *h, t_heap_node node);
t_heap_node heap_pop(t_heap *h);

#endif
