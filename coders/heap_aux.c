/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/heap.h"
#include <stdlib.h>

void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_init(t_heap *h)
{
	h->size = 0;
	h->capacity = 16;
	h->nodes = malloc(sizeof(t_heap_node) * h->capacity);
}

void	heap_destroy(t_heap *h)
{
	free(h->nodes);
	h->nodes = NULL;
	h->size = 0;
	h->capacity = 0;
}
