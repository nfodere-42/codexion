/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfodere- <>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:55 by nfodere-          #+#    #+#             */
/*   Updated: 2025/09/22 14:22:07 by nfodere-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"
#include "../include/scheduler.h"
#include <string.h>
#include <stdlib.h>

int	parse_scheduler(const char *s, t_scheduler_type *out)
{
	if (strcmp(s, "fifo") == 0)
		*out = SCH_FIFO;
	else if (strcmp(s, "edf") == 0)
		*out = SCH_EDF;
	else
		return (1);
	return (0);
}

int	parse_args(int ac, char **av, t_config *cfg)
{
	if (ac != 9)
		return (1);
	cfg->number_of_coders = atoi(av[1]);
	cfg->time_to_burnout = atol(av[2]);
	cfg->time_to_compile = atol(av[3]);
	cfg->time_to_debug = atol(av[4]);
	cfg->time_to_refactor = atol(av[5]);
	cfg->number_of_compiles_required = atoi(av[6]);
	cfg->dongle_cooldown = atol(av[7]);
	if (parse_scheduler(av[8], &cfg->scheduler))
		return (1);
	if (cfg->number_of_coders <= 0)
		return (1);
	return (0);
}
