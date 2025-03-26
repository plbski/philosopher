#ifndef PHILO_H
# define PHILO_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include"libft/libft.h"

typedef struct s_data{
	int time_to_die;
	int time_to_sleep;
	int time_to_eat;
	int nb_philo;
} t_data;

typedef struct s_philo{
	int*				share_count;
	pthread_mutex_t* 	mutex;
	t_data				data;
	int					id;
	int*				stop_flag;
	struct timeval		*last_update;
	int*					*r_fork;
} t_philo;


#endif