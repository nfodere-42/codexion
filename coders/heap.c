#include "../include/heap.h"
#include <stdlib.h>

static void swap_nodes(t_heap_node *a, t_heap_node *b)
{
    t_heap_node tmp = *a;
    *a = *b;
    *b = tmp;
}

void    heap_init(t_heap *h)
{
    h->size = 0;
    h->capacity = 16;
    h->nodes = malloc(sizeof(t_heap_node) * h->capacity);
}

void    heap_destroy(t_heap *h)
{
    free(h->nodes);
    h->nodes = NULL;
    h->size = 0;
    h->capacity = 0;
}

static void heap_grow(t_heap *h)
{
    h->capacity *= 2;
    h->nodes = realloc(h->nodes, sizeof(t_heap_node) * h->capacity);
}

static void heap_up(t_heap *h, int idx)
{
    int parent;

    while (idx > 0)
    {
        parent = (idx - 1) / 2;
        if (h->nodes[idx].priority >= h->nodes[parent].priority)
            break;
        swap_nodes(&h->nodes[idx], &h->nodes[parent]);
        idx = parent;
    }
}

static void heap_down(t_heap *h, int idx)
{
    int left, right, smallest;

    while (1)
    {
        left = idx * 2 + 1;
        right = idx * 2 + 2;
        smallest = idx;
        if (left < h->size &&
            h->nodes[left].priority < h->nodes[smallest].priority)
            smallest = left;
        if (right < h->size &&
            h->nodes[right].priority < h->nodes[smallest].priority)
            smallest = right;
        if (smallest == idx)
            break;
        swap_nodes(&h->nodes[idx], &h->nodes[smallest]);
        idx = smallest;
    }
}

void    heap_push(t_heap *h, t_heap_node node)
{
    if (h->size == h->capacity)
        heap_grow(h);
    h->nodes[h->size] = node;
    heap_up(h, h->size);
    h->size++;
}

t_heap_node heap_pop(t_heap *h)
{
    t_heap_node top = h->nodes[0];

    h->size--;
    h->nodes[0] = h->nodes[h->size];
    heap_down(h, 0);
    return top;
}
