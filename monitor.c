/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plbuet <plbuet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:43:40 by pbuet             #+#    #+#             */
/*   Updated: 2025/04/23 12:55:56 by plbuet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	check_time(void *arg, int i)
{
	t_philo					*args;
	unsigned int			t;

	args = (t_philo *)arg;
	pthread_mutex_lock(&args[i].data_mutex);
	t = get_time();
	if ((((t - args[i].last_update) >= args->data->time_to_die)
			&& (args[i].eaten != 1)))
	{
		pthread_mutex_lock(&args[i].data->stop_mutex);
		args->data->stop_flag = 1;
		pthread_mutex_unlock(&args[i].data->stop_mutex);
		pthread_mutex_unlock(&args[i].data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&args[i].data_mutex);
	return (0);
}

void	*monitor(void *arg)
{
	t_philo					*args;
	int						i;
	int						finish;

	args = (t_philo *)arg;
	while (1)
	{
		usleep(500);
		i = 0;
		finish = 0;
		while (i < args->data->nb_philo - 1)
		{
			pthread_mutex_lock(&args[i].data_mutex);
			if (args[i].finish == 1)
				finish ++;
			pthread_mutex_unlock(&args[i].data_mutex);
			if (check_time(args, i))
				return ((void *)&args[i].id);
			i ++;
		}
		if (finish == i)
			return (NULL);
	}
}
