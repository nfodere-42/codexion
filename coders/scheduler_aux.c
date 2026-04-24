/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/scheduler.h"
#include "../include/heap.h"
#include <unistd.h>

int	compare_priorities(long a, long b)
{
	if (a < b)
		return (-1);
	if (a > b)
		return (1);
	return (0);
}

static void	write_number(long n)
{
	char	buf[32];
	int		i;

	if (n == 0)
	{
		write(1, "0", 1);
		return ;
	}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	i = 31;
	buf[i] = '\0';
	i--;
	while (n > 0)
	{
		buf[i] = '0' + (n % 10);
		n /= 10;
		i--;
	}
	write(1, &buf[i + 1], 31 - i - 1);
}

void	scheduler_print_queue(t_scheduler *s)
{
	int	i;

	pthread_mutex_lock(&s->mutex);
	i = 0;
	while (i < s->queue.size)
	{
		write(1, "Node ", 5);
		write_number(i);
		write(1, ": coder ", 8);
		write_number(s->queue.nodes[i].coder->id);
		write(1, ", priority ", 11);
		write_number(s->queue.nodes[i].priority);
		write(1, "\n", 1);
		i++;
	}
	pthread_mutex_unlock(&s->mutex);
}
