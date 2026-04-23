#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* Scheduler type */
typedef enum e_scheduler_type
{
    SCH_FIFO,
    SCH_EDF
}   t_scheduler_type;

/* Coder state */
typedef enum e_coder_state
{
    STATE_COMPILING,
    STATE_DEBUGGING,
    STATE_REFACTORING,
    STATE_BURNED
}   t_coder_state;

/* Forward declaration */
struct s_ctx;

/* Configuration */
typedef struct s_config
{
    int                 number_of_coders;
    long                time_to_burnout;
    long                time_to_compile;
    long                time_to_debug;
    long                time_to_refactor;
    int                 number_of_compiles_required;
    long                dongle_cooldown;
    t_scheduler_type    scheduler;
}   t_config;

/* Dongle */
typedef struct s_dongle
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    long            available_at;
    int             held_by;
}   t_dongle;

/* Coder */
typedef struct s_coder
{
    int             id;
    pthread_t       thread;
    long            last_compile_start;
    int             compile_count;
    t_coder_state   state;
    struct s_ctx    *ctx;
}   t_coder;

/* Global context */
typedef struct s_ctx
{
    t_config        cfg;
    t_dongle        *dongles;
    t_coder         *coders;

    long            start_time;
    int             stop_simulation;

    pthread_mutex_t log_mutex;
    pthread_mutex_t sim_mutex;

    pthread_t       monitor_thread;
}   t_ctx;

/* Utils */
long    get_timestamp_ms(void);
void safe_usleep(long ms, t_ctx *ctx);
void    log_action(t_ctx *ctx, int id, const char *msg);

#endif
