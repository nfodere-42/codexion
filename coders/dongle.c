#include "../include/codexion.h"
#include "../include/dongle.h"
#include "../include/utils.h"

int init_dongles(t_ctx *ctx)
{
    int i;

    for (i = 0; i < ctx->cfg.number_of_coders; i++)
    {
        if (pthread_mutex_init(&ctx->dongles[i].mutex, NULL) != 0)
            return (1);
        if (pthread_cond_init(&ctx->dongles[i].cond, NULL) != 0)
            return (1);
        ctx->dongles[i].available_at = 0;
        ctx->dongles[i].held_by = 0;
    }
    return (0);
}

void    destroy_dongle(t_dongle *d)
{
    pthread_mutex_destroy(&d->mutex);
    pthread_cond_destroy(&d->cond);
}

static int  wait_for_dongle(t_ctx *ctx, t_dongle *d, int coder_id)
{
    long now;

    pthread_mutex_lock(&d->mutex);
    while (1)
    {
        pthread_mutex_lock(&ctx->sim_mutex);
        if (ctx->stop_simulation)
        {
            pthread_mutex_unlock(&ctx->sim_mutex);
            pthread_mutex_unlock(&d->mutex);
            return 1;
        }
        pthread_mutex_unlock(&ctx->sim_mutex);
        now = get_timestamp_ms();
        if (now >= d->available_at && d->held_by == 0)
        {
            d->held_by = coder_id;
            pthread_mutex_unlock(&d->mutex);
            return 0;
        }
        pthread_cond_wait(&d->cond, &d->mutex);
    }
}

static void release_one(t_ctx *ctx, t_dongle *d)
{
    pthread_mutex_lock(&d->mutex);
    d->held_by = 0;
    d->available_at = get_timestamp_ms() + ctx->cfg.dongle_cooldown;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}

int take_dongles(t_coder *c)
{
    t_ctx *ctx = c->ctx;
    int left = c->id - 1;
    int right = (c->id % ctx->cfg.number_of_coders);

    log_action(ctx, c->id, "has taken a dongle");
    if (wait_for_dongle(ctx, &ctx->dongles[left], c->id))
        return (1);
    log_action(ctx, c->id, "has taken a dongle");
    if (wait_for_dongle(ctx, &ctx->dongles[right], c->id))
        return (1);

    return (0);
}

void    release_dongles(t_coder *c)
{
    t_ctx *ctx = c->ctx;
    int left = c->id - 1;
    int right = (c->id % ctx->cfg.number_of_coders);

    release_one(ctx, &ctx->dongles[left]);
    release_one(ctx, &ctx->dongles[right]);
}
