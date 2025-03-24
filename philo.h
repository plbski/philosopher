#ifndef PHILO_H
# define PHILO_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include"libft/libft.h"

typedef struct s_data{
	int time_to_die;
	int time_to_sleep;
	int time_to_eat;
} t_data;

typedef struct s_philo{
	int*				share_count;
	pthread_mutex_t* 	mutex;
	t_data				data;
	int					id;
} t_philo;


#endif