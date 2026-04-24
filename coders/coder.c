/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"
#include "../include/coder.h"
#include "../include/dongle.h"
#include "../include/utils.h"

static void	do_compile(t_coder *c)
{
	t_ctx	*ctx;

	ctx = c->ctx;
	if (simulation_stopped(ctx))
		return ;
	log_action(ctx, c->id, "is compiling");
	c->state = STATE_COMPILING;
	c->last_compile_start = get_timestamp_ms();
	safe_usleep(ctx->cfg.time_to_compile, ctx);
	if (simulation_stopped(ctx))
		return ;
	c->compile_count++;
}

static void	do_debug(t_coder *c)
{
	t_ctx	*ctx;

	ctx = c->ctx;
	if (simulation_stopped(ctx))
		return ;
	log_action(ctx, c->id, "is debugging");
	c->state = STATE_DEBUGGING;
	safe_usleep(ctx->cfg.time_to_debug, ctx);
}

static void	do_refactor(t_coder *c)
{
	t_ctx	*ctx;

	ctx = c->ctx;
	if (simulation_stopped(ctx))
		return ;
	log_action(ctx, c->id, "is refactoring");
	c->state = STATE_REFACTORING;
	safe_usleep(ctx->cfg.time_to_refactor, ctx);
}

void	*coder_thread(void *arg)
{
	t_coder	*c;
	t_ctx	*ctx;

	c = arg;
	ctx = c->ctx;
	while (!simulation_stopped(ctx))
	{
		if (c->compile_count >= ctx->cfg.number_of_compiles_required
			|| take_dongles(c))
			break ;
		if (simulation_stopped(ctx))
		{
			release_dongles(c);
			break ;
		}
		do_compile(c);
		release_dongles(c);
		if (simulation_stopped(ctx))
			break ;
		do_debug(c);
		if (simulation_stopped(ctx))
			break ;
		do_refactor(c);
	}
	return (NULL);
}

int	init_coders(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->cfg.number_of_coders)
	{
		ctx->coders[i].id = i + 1;
		ctx->coders[i].ctx = ctx;
		ctx->coders[i].compile_count = 0;
		ctx->coders[i].state = STATE_REFACTORING;
		ctx->coders[i].last_compile_start = ctx->start_time;
		i++;
	}
	return (0);
}
