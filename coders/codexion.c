#include "../include/codexion.h"
#include "../include/coder.h"
#include "../include/dongle.h"
#include "../include/monitor.h"
#include "../include/utils.h"
#include "../include/scheduler.h"

static int	parse_scheduler(const char *s, t_scheduler_type *out)
{
	if (strcmp(s, "fifo") == 0)
		*out = SCH_FIFO;
	else if (strcmp(s, "edf") == 0)
		*out = SCH_EDF;
	else
		return (1);
	return (0);
}

static int	parse_args(int ac, char **av, t_config *cfg)
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

static int	init_ctx(t_ctx *ctx, t_config *cfg)
{
	memset(ctx, 0, sizeof(t_ctx));
	ctx->cfg = *cfg;
	pthread_mutex_init(&ctx->log_mutex, NULL);
	pthread_mutex_init(&ctx->sim_mutex, NULL);
	ctx->coders = malloc(sizeof(t_coder) * cfg->number_of_coders);
	ctx->dongles = malloc(sizeof(t_dongle) * cfg->number_of_coders);
	if (!ctx->coders || !ctx->dongles)
		return (1);
	ctx->start_time = get_timestamp_ms();
	if (init_dongles(ctx))
		return (1);
	if (init_coders(ctx))
		return (1);
	return (0);
}

static void	start_threads(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->cfg.number_of_coders)
	{
		pthread_create(&ctx->coders[i].thread, NULL,
			coder_thread, &ctx->coders[i]);
		i++;
	}
	pthread_create(&ctx->monitor_thread, NULL, monitor_thread, ctx);
}

static void	join_threads(t_ctx *ctx)
{
	for (int i = 0; i < ctx->cfg.number_of_coders; i++)
		pthread_join(ctx->coders[i].thread, NULL);
	pthread_join(ctx->monitor_thread, NULL);
}

static void free_ctx(t_ctx *ctx)
{
	for (int i = 0; i < ctx->cfg.number_of_coders; i++)
		destroy_dongle(&ctx->dongles[i]);
	pthread_mutex_destroy(&ctx->log_mutex);
	pthread_mutex_destroy(&ctx->sim_mutex);
	free(ctx->coders);
	free(ctx->dongles);
}

int	main(int ac, char **av)
{
	t_config	cfg;
	t_ctx		ctx;

	if (parse_args(ac, av, &cfg))
		return (fprintf(stderr, "Invalid arguments\n"), 1);
	if (init_ctx(&ctx, &cfg))
		return (fprintf(stderr, "Initialization error\n"), 1);
	start_threads(&ctx);
	join_threads(&ctx);
	free_ctx(&ctx);
	return (0);
}
