#ifndef DONGLE_H
#define DONGLE_H

#include "codexion.h"

int     init_dongles(t_ctx *ctx);
void    destroy_dongle(t_dongle *d);

int     take_dongles(t_coder *c);
void    release_dongles(t_coder *c);

#endif
