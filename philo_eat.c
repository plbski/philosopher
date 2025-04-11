/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbuet <pbuet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:08 by pbuet             #+#    #+#             */
/*   Updated: 2025/04/11 14:03:03 by pbuet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	unlock_fork(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	if (args->l_fork < args->r_fork)
	{
		pthread_mutex_unlock(args->r_fork);
		pthread_mutex_unlock(args->l_fork);
	}
	else
	{
		pthread_mutex_unlock(args->l_fork);
		pthread_mutex_unlock(args->r_fork);
	}
}

void	lock_fork(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	if (args->l_fork < args->r_fork)
	{
		pthread_mutex_lock(args->l_fork);
		pthread_mutex_lock(args->r_fork);
	}
	else
	{
		pthread_mutex_lock(args->r_fork);
		pthread_mutex_lock(args->l_fork);
	}
	print_mute(args, 0);
}

void	eat(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	lock_fork(args);
	pthread_mutex_lock(&args->data_mutex);
	args->last_update = get_time();
	args->eaten = 1;
	print_mute(args, 1);
	pthread_mutex_unlock(&args->data_mutex);
	usleep(args->data->time_to_eat);
	pthread_mutex_lock(&args->data_mutex);
	unlock_fork(args);
	args->eaten = 0;
	args->share_count ++;
	if (args->share_count == args->data->nb_eat)
	{
		args->finish = 1;
		pthread_mutex_unlock(&args->data_mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&args->data_mutex);
}
