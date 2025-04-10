#ifndef PHILO_H
# define PHILO_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_data{
	unsigned int	time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	int				nb_philo;
	int				nb_eat;
	int				stop_flag;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
} t_data;

typedef struct s_philo{
	int					share_count;
	int					eaten;
	int					id;
	unsigned int		last_update;
	pthread_mutex_t*	r_fork;
	pthread_mutex_t*	l_fork;
	pthread_mutex_t		data_mutex;
	t_data				*data;
} t_philo;

int	init_data(t_data *data, char **v);

//utils
long			get_time();
pthread_mutex_t	*fork_init(int nb_philo);
int 			ft_atoi(char *s);
void			print_mute(void *arg, int note, int died);

//philo_eat
void			eat(void *arg);
void			manage_impair(void *arg, int manage);

#endif