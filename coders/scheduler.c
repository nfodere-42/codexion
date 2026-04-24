/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"
#include "../include/scheduler.h"
#include "../include/heap.h"
#include "../include/utils.h"

void	scheduler_init(t_scheduler *s, t_scheduler_type type)
{
	s->type = type;
	heap_init(&s->queue);
	pthread_mutex_init(&s->mutex, NULL);
	pthread_cond_init(&s->cond, NULL);
}

void	scheduler_destroy(t_scheduler *s)
{
	pthread_mutex_destroy(&s->mutex);
	pthread_cond_destroy(&s->cond);
	heap_destroy(&s->queue);
}

static long	get_priority(t_coder *c)
{
	if (c->ctx->cfg.scheduler == SCHED_FIFO)
		return (get_timestamp_ms());
	return (c->last_compile_start + c->ctx->cfg.time_to_burnout);
}

void	scheduler_request(t_scheduler *s, t_coder *c)
{
	t_heap_node	node;

	node.coder = c;
	node.priority = get_priority(c);
	pthread_mutex_lock(&s->mutex);
	heap_push(&s->queue, node);
	pthread_cond_broadcast(&s->cond);
	pthread_mutex_unlock(&s->mutex);
}

t_coder	*scheduler_pop(t_scheduler *s)
{
	t_heap_node	node;

	pthread_mutex_lock(&s->mutex);
	while (s->queue.size == 0)
		pthread_cond_wait(&s->cond, &s->mutex);
	node = heap_pop(&s->queue);
	pthread_mutex_unlock(&s->mutex);
	return (node.coder);
}
