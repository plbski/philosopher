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

void	print_mute(void *arg, int note, int died)
{
	t_philo	*args;
	long	time;
	int		stop;


	args =(t_philo *)arg;
	time = get_time();
	pthread_mutex_lock(&args->data->stop_mutex);
	stop = args->data->stop_flag;
	pthread_mutex_unlock(&args->data->stop_mutex);
	if (stop != 1)
	{
		pthread_mutex_lock(&args->data->print_mutex);
		if (note == 0)
				printf("%ld, %d has taken a fork\n", time, args->id);
		else if(note == 1)
			printf("%ld %d is eating\n", time, args->id);
		else if(note == 2)
			printf("%ld %d is thinking\n", time, args->id);
		else if(note == 3)
			printf("%ld %d is sleeping\n", time, args->id);
		else
			printf("%ld %d is died\n", time, died);
		pthread_mutex_unlock(&args->data->print_mutex);
	}
}

int	init_data(t_data *data, char **v)
{
	data->nb_philo = ft_atoi(v[1]);
	data->time_to_die = ft_atoi(v[2]);
	data->time_to_eat = ft_atoi(v[3]) * 1000;
	data->time_to_sleep = ft_atoi(v[4]) * 1000;
	data->stop_flag = 0;
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	if (data->time_to_die <= 0 || data->time_to_eat <= 0 || data->time_to_sleep <=0 || data->nb_philo <= 0)
		return(1);
	return(0);
}
