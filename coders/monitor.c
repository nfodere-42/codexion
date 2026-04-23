#include "../include/codexion.h"
#include "../include/monitor.h"
#include "../include/utils.h"

static int  all_finished(t_ctx *ctx)
{
    int i;

    for (i = 0; i < ctx->cfg.number_of_coders; i++)
    {
        if (ctx->coders[i].compile_count <
            ctx->cfg.number_of_compiles_required)
            return 0;
    }
    return 1;
}

static void stop_simulation(t_ctx *ctx)
{
    pthread_mutex_lock(&ctx->sim_mutex);
    ctx->stop_simulation = 1;
    pthread_mutex_unlock(&ctx->sim_mutex);

    for (int i = 0; i < ctx->cfg.number_of_coders; i++)
    {
        pthread_mutex_lock(&ctx->dongles[i].mutex);
        pthread_cond_broadcast(&ctx->dongles[i].cond);
        pthread_mutex_unlock(&ctx->dongles[i].mutex);
    }
}

static int  check_burnout(t_ctx *ctx, t_coder *c)
{
    long now = get_timestamp_ms();
    long deadline = c->last_compile_start + ctx->cfg.time_to_burnout;

    if (now > deadline)
    {
        log_action(ctx, c->id, "burned out");
        stop_simulation(ctx);
        return 1;
    }
    return 0;
}

void    *monitor_thread(void *arg)
{
    t_ctx *ctx = arg;

    while (1)
    {
        if (all_finished(ctx))
        {
            stop_simulation(ctx);
            return NULL;
        }
        for (int i = 0; i < ctx->cfg.number_of_coders; i++)
        {
            if (check_burnout(ctx, &ctx->coders[i]))
                return NULL;
        }
        usleep(1000);
    }
    return NULL;
}
