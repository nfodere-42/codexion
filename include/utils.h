#ifndef UTILS_H
# define UTILS_H

# include "codexion.h"

long    get_timestamp_ms(void);
void    safe_usleep(long ms, t_ctx *ctx);
int     simulation_stopped(t_ctx *ctx);
void    log_action(t_ctx *ctx, int id, const char *msg);

#endif

