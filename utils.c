#include"philo.h"

long	get_time()
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

pthread_mutex_t *fork_init(int nb_philo)
{
	int				i;
	pthread_mutex_t	*fork;

	i = 0;
	fork = malloc(sizeof(pthread_mutex_t) * nb_philo);
	while (i < nb_philo)
	{
		pthread_mutex_init(&fork[i], NULL);
		i ++;
	}
	return(fork);
}