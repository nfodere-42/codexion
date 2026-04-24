/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"
#include "../include/utils.h"
#include <unistd.h>

long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	safe_usleep(long ms, t_ctx *ctx)
{
	long	start;

	start = get_timestamp_ms();
	while (!simulation_stopped(ctx)
		&& get_timestamp_ms() - start < ms)
		usleep(100);
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

void	log_action(t_ctx *ctx, int id, const char *msg)
{
	long	ts;

	pthread_mutex_lock(&ctx->log_mutex);
	ts = get_timestamp_ms() - ctx->start_time;
	write_number(ts);
	write(1, " ", 1);
	write_number(id);
	write(1, " ", 1);
	write(1, msg, strlen(msg));
	write(1, "\n", 1);
	pthread_mutex_unlock(&ctx->log_mutex);
}
