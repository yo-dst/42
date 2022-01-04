#ifndef PHILO_H
# define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

# define NOT_READY '0'
# define READY '1'
# define TAKEN '0'
# define AVAILABLE '1'
# define TAKE_FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DIE 4

typedef struct	s_philo_status
{
	int	n;
}	t_philo_status;

typedef struct	s_ctx
{
	int				n;
	struct timeval	start_time;
	int				nb_philo;
	float			t_death;
	float			t_eat;
	float			t_sleep;
	int				nb_meal_per_philo;
	char			*forks;
	char			*philo;
	int				is_running;
	pthread_mutex_t	lock;
}	t_ctx;

float	time_diff(struct timeval start, struct timeval end);
int		on_error(int code);
void	print_log(float time, int n, int action);

// parse
int		parse(int ac, char **av, t_ctx *ctx);

// utils
int		ft_atoi(const char *str);
float	ft_atof(char *s);

// dev
void	print_forks(t_ctx *ctx);
void	print_ctx(t_ctx *ctx);

#endif