#include "../include/codexion.h"
#include "../include/utils.h"

long    get_timestamp_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void safe_usleep(long ms, t_ctx *ctx)
{
    long start = get_timestamp_ms();

    while (!simulation_stopped(ctx) &&
           get_timestamp_ms() - start < ms)
    {
        usleep(100);
    }
}

void    log_action(t_ctx *ctx, int id, const char *msg)
{
    long    ts;

    pthread_mutex_lock(&ctx->log_mutex);
    ts = get_timestamp_ms() - ctx->start_time;
    printf("%ld %d %s\n", ts, id, msg);
    fflush(stdout);
    pthread_mutex_unlock(&ctx->log_mutex);
}
