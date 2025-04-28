/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plbuet <plbuet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:08 by pbuet             #+#    #+#             */
/*   Updated: 2025/04/28 08:49:58 by plbuet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	test(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	pthread_mutex_lock(&args->data_mutex);
	if (args->data->nb_philo == 1)
	{
		args->finish = 1;
		pthread_mutex_unlock(&args->data_mutex);
		print_mute(args, "died", 0);
		return (1);
	}
	pthread_mutex_unlock(&args->data_mutex);
	return (0);
}

void	unlock_fork(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	if (args->id % 2 == 0)
	{
		pthread_mutex_unlock(args->l_fork);
		pthread_mutex_unlock(args->r_fork);
	}
	else
	{
		pthread_mutex_unlock(args->r_fork);
		pthread_mutex_unlock(args->l_fork);
	}
}

void	lock_fork(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	if (args->id % 2 == 0)
	{
		pthread_mutex_lock(args->r_fork);
		print_mute(args, "has taken a fork", 0);
		pthread_mutex_lock(args->l_fork);
	}
	else
	{
		pthread_mutex_lock(args->l_fork);
		print_mute(args, "has taken a fork", 0);
		pthread_mutex_lock(args->r_fork);
	}
	print_mute(args, "has taken a fork", 0);
}

int	eat(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	lock_fork(args);
	pthread_mutex_lock(&args->data_mutex);
	args->last_update = get_time();
	args->eaten = 1;
	print_mute(args, "is eating", 0);
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
		return (1);
	}
	pthread_mutex_unlock(&args->data_mutex);
	print_mute(args, "is thinking", 0);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*args;
	int		stop;

	args = (t_philo *)arg;
	if (test(args))
		return (NULL);
	if ((args->id % 2) != 0)
		usleep(20);
	while (1)
	{
		if ((args->id % 2) != 0)
			usleep(200);
		if (eat(args))
			break;
		pthread_mutex_lock(&args->data->stop_mutex);
		stop = args->data->stop_flag;
		pthread_mutex_unlock(&args->data->stop_mutex);
		if (stop == 1)
			break;
		print_mute(args, "is sleeping", 0);
		usleep(args->data->time_to_sleep);
	}
	return (NULL);
}