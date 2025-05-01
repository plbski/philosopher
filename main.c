/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plbuet <plbuet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:36:30 by pbuet             #+#    #+#             */
/*   Updated: 2025/05/01 13:05:24 by plbuet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	red_flag(pthread_t *philo, t_philo *args, int nb_philo)
{
	int		i;

	i = 0;
		pthread_mutex_lock(&args->data->stop_mutex);
		args->data->stop_flag = 1;
		pthread_mutex_unlock(&args->data->stop_mutex);
		while (i < nb_philo)
		{
			pthread_join(philo[i], NULL);
			i++;
		}
}
void	create_philo(pthread_t *philo, t_philo *args)
{
	int			i;
	pthread_t	monitoring;
	int			*dead;

	i = 0;
	while (i < args->data->nb_philo)
	{
		if (pthread_create(&philo[i], NULL, routine, &args[i]) != 0)
			break;;
		i++;
	}
	if ((i < args->data->nb_philo) || (pthread_create(&monitoring, NULL, monitor, args) != 0))
	{
		red_flag(philo, args, i);
		return ;
	}
	pthread_join(monitoring, (void **)&dead);
	i = 0;
	if (dead != NULL)
		print_mute(&args[*dead -1], "died", 1);
	while (i < args->data->nb_philo)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
}

int	init_philo(t_philo *args, pthread_mutex_t *fork, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		args[i].data = data;
		args[i].id = i + 1;
		args[i].share_count = 0;
		args[i].eaten = 0;
		args[i].last_update = get_time();
		args[i].r_fork = &fork[i];
		args[i].l_fork = &fork[(i + 1) % data->nb_philo];
		args[i].finish = 0;
		if (pthread_mutex_init(&args[i].data_mutex, NULL) != 0)
		{
			while (i >= 0)
			{
				pthread_mutex_destroy(&args[i].data_mutex);
				i --;
			}
			return(1);
		}
		i ++;
	}
	return (0);
}

void	freedest(t_data data, pthread_mutex_t *fork,
					pthread_t *philo, t_philo *args)
{
	int	i;

	i = 0;
	while (i < data.nb_philo)
	{
		pthread_mutex_destroy(&fork[i]);
		pthread_mutex_destroy(&args[i].data_mutex);
		i ++;
	}
	pthread_mutex_destroy(&data.print_mutex);
	free(fork);
	free(philo);
	free(args);
}

int	main(int c, char **v)
{
	t_data			data;
	pthread_mutex_t	*fork;
	pthread_t		*philo;
	t_philo			*args;

	if (c < 5 || c > 6)
		return (1);
	if (init_data(&data, v, c) != 0)
		{printf("exit \n");
		return (1);}
	if ((fork = fork_init(data.nb_philo)) == NULL)
		return (1);
	philo = malloc(sizeof(pthread_t) * data.nb_philo + 1);
	args = malloc(sizeof(t_philo) * data.nb_philo + 1);
	if (!philo || !args)
		return (1);
	if (!init_philo(args, fork, &data))
		create_philo(philo, args);
	freedest(data, fork, philo, args);
	return (0);
}
