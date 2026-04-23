#include "../include/codexion.h"
#include "../include/coder.h"
#include "../include/dongle.h"
#include "../include/utils.h"

int  simulation_stopped(t_ctx *ctx)
{
    int stopped;

    pthread_mutex_lock(&ctx->sim_mutex);
    stopped = ctx->stop_simulation;
    pthread_mutex_unlock(&ctx->sim_mutex);
    return (stopped);
}

static void do_compile(t_coder *c)
{
    t_ctx *ctx = c->ctx;

    if (simulation_stopped(ctx))
        return;
    log_action(ctx, c->id, "is compiling");
    c->state = STATE_COMPILING;
    c->last_compile_start = get_timestamp_ms();
    safe_usleep(ctx->cfg.time_to_compile, ctx);
    if (simulation_stopped(ctx))
        return;
    c->compile_count++;
}

static void do_debug(t_coder *c)
{
    t_ctx *ctx = c->ctx;

    if (simulation_stopped(ctx))
        return;
    log_action(ctx, c->id, "is debugging");
    c->state = STATE_DEBUGGING;
    safe_usleep(ctx->cfg.time_to_debug, ctx);
}

static void do_refactor(t_coder *c)
{
    t_ctx *ctx = c->ctx;

    if (simulation_stopped(ctx))
        return;
    log_action(ctx, c->id, "is refactoring");
    c->state = STATE_REFACTORING;
    safe_usleep(ctx->cfg.time_to_refactor, ctx);
}

void    *coder_thread(void *arg)
{
    t_coder *c = arg;
    t_ctx   *ctx = c->ctx;

	while (!simulation_stopped(ctx))
	{
	    if (c->compile_count >= ctx->cfg.number_of_compiles_required)
		break;
	    if (take_dongles(c))
		break;
	    if (simulation_stopped(ctx))
	    {
		release_dongles(c);
		break;
	    }
	    do_compile(c);
	    release_dongles(c);
	    if (simulation_stopped(ctx))
		break;
	    do_debug(c);
	    if (simulation_stopped(ctx))
		break;
	    do_refactor(c);
	}
    return (NULL);
}

int init_coders(t_ctx *ctx)
{
    for (int i = 0; i < ctx->cfg.number_of_coders; i++)
    {
        ctx->coders[i].id = i + 1;
        ctx->coders[i].ctx = ctx;
        ctx->coders[i].compile_count = 0;
        ctx->coders[i].state = STATE_REFACTORING;
        ctx->coders[i].last_compile_start = ctx->start_time;
    }
    return 0;
}

