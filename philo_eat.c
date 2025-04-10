/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbuet <pbuet@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:08 by pbuet             #+#    #+#             */
/*   Updated: 2025/04/10 15:31:03 by pbuet            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	manage_fork(void *arg, int manage)
{
	t_philo	*args;

	args = (t_philo *)arg;
	if (manage == 0)
	{
		pthread_mutex_lock(args->l_fork);
		pthread_mutex_lock(args->r_fork);
		print_mute(args, 0, 0);
	}
	else
	{
		pthread_mutex_unlock(args->l_fork);
		pthread_mutex_unlock(args->r_fork);
		print_mute(args, 0, 0);
	}
}

void	eat(void *arg)
{
	t_philo	*args;

	args = (t_philo *)arg;
	manage_fork(args, 0);
	pthread_mutex_lock(&args->data_mutex);
	args->eaten = 1;
	print_mute(args, 1, 0);
	pthread_mutex_unlock(&args->data_mutex);
	usleep(args->data->time_to_eat);
	pthread_mutex_lock(&args->data_mutex);
	args->last_update = get_time();
	args->eaten = 0;
	args->share_count ++;
	pthread_mutex_unlock(&args->data_mutex);
	manage_fork(args, 1);
}
