/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

typedef enum e_scheduler_type
{
	SCH_FIFO,
	SCH_EDF
}	t_scheduler_type;

typedef enum e_coder_state
{
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING,
	STATE_BURNED
}	t_coder_state;

struct	s_ctx;

typedef struct s_config
{
	t_scheduler_type	scheduler;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				dongle_cooldown;
	int					number_of_compiles_required;
	int					number_of_coders;
}	t_config;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long			available_at;
	int				held_by;
}	t_dongle;

typedef struct s_coder
{
	struct s_ctx	*ctx;
	t_coder_state	state;
	pthread_t		thread;
	long			last_compile_start;
	int				id;
	int				compile_count;
}	t_coder;

typedef struct s_ctx
{
	t_config		cfg;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	sim_mutex;
	pthread_t		monitor_thread;
	long			start_time;
	int				stop_simulation;
}	t_ctx;

long	get_timestamp_ms(void);
void	safe_usleep(long ms, t_ctx *ctx);
void	log_action(t_ctx *ctx, int id, const char *msg);
int		parse_scheduler(const char *s, t_scheduler_type *out);
int		parse_args(int ac, char **av, t_config *cfg);

#endif
