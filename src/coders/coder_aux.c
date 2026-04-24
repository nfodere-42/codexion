/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/coder.h"
#include "../include/codexion.h"
#include <pthread.h>

int	simulation_stopped(t_ctx *ctx)
{
	int	stopped;

	pthread_mutex_lock(&ctx->sim_mutex);
	stopped = ctx->stop_simulation;
	pthread_mutex_unlock(&ctx->sim_mutex);
	return (stopped);
}
