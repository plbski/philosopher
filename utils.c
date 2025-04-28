/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plbuet <plbuet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:48:01 by pbuet             #+#    #+#             */
/*   Updated: 2025/04/28 11:05:13 by plbuet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	ft_atoi(char *s)
{
	int				i;
	unsigned int	nb;

	i = 0;
	nb = 0;
	while (s[i])
	{
		if (s[i] >= 48 && s[i] <= 57)
			nb = (nb * 10) + (s[i] - '0');
		else
			return (-1);
		if (i >= 11)
			return (-1);
		i ++;
	}
	return (nb);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

pthread_mutex_t	*fork_init(int nb_philo)
{
	int				i;
	pthread_mutex_t	*fork;

	i = 0;
	fork = malloc(sizeof(pthread_mutex_t) * nb_philo);
	while (i < nb_philo)
	{
		if (pthread_mutex_init(&fork[i], NULL))
			break;
		i ++;
	}
	if (i < nb_philo)
	{
		while (i <= 0)
		{
			pthread_mutex_destroy(&fork[i]);
			i --;
		}
		return(NULL);
	}
	return (fork);
}

void	print_mute(void *arg, char *s, int force)
{
	t_philo	*args;
	int		stop;

	args = (t_philo *)arg;
	pthread_mutex_lock(&args->data->stop_mutex);
	stop = args->data->stop_flag;
	pthread_mutex_unlock(&args->data->stop_mutex);
	if (stop != 1 || force == 1)
	{
		pthread_mutex_lock(&args->data->print_mutex);
		printf("%ld %d %s\n", get_time() -  args->data->start, args->id, s);
		pthread_mutex_unlock(&args->data->print_mutex);
	}
}

int	init_data(t_data *data, char **v, int c)
{
	data->nb_philo = ft_atoi(v[1]);
	data->time_to_die = ft_atoi(v[2]);
	data->time_to_eat = ft_atoi(v[3]) * 1000;
	data->time_to_sleep = ft_atoi(v[4]) * 1000;
	data->stop_flag = 0;
	data->start = get_time();
	if (c == 6)
		data->nb_eat = ft_atoi(v[5]);
	else
		data->nb_eat = 0;
	if ((pthread_mutex_init(&data->stop_mutex, NULL) != 0))
	{
		pthread_mutex_destroy(&data->stop_mutex);
		return(1);
	}
	if ((pthread_mutex_init(&data->print_mutex, NULL) != 0))
	{
		pthread_mutex_destroy(&data->print_mutex);
		return(1);
	}
	if (data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || data->nb_philo <= 0
		|| data->nb_eat <= 0)
		return (1);
	return (0);
}
