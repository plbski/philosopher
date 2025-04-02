#include"philo.h"

int ft_atoi(char *s)
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
			return(-1);
		i ++;
	}
	return(nb);
}
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

void manage_fork(void *arg, int manage)
{
	t_philo *args;

	args =(t_philo *)arg;
		if (args->id % 2 == 0)
		{
			if (manage == 0)
			{
				pthread_mutex_lock(args->r_fork); 
				pthread_mutex_lock(args->l_fork);
			}
			else
			{
				pthread_mutex_unlock(args->r_fork); 
				pthread_mutex_unlock(args->l_fork);
			}
		}
		else
		{
			if (manage == 0)
			{
				pthread_mutex_lock(args->l_fork); 
				pthread_mutex_lock(args->r_fork);
			}
			else
			{
				pthread_mutex_unlock(args->l_fork); 
				pthread_mutex_unlock(args->r_fork);
			} 
		}
}

