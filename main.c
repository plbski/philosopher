#include "philo.h"

void	*monitor(void *arg)
{
	t_philo 					*args;
	int							i;
	unsigned int				t;

	args = (t_philo *)arg;
	while(1)
	{
		usleep(500);
		i = 0;
		while (i < args->data->nb_philo - 1)
		{
			pthread_mutex_lock(&args[i].data_mutex);
			t = get_time();
			if (((t - args[i].last_update) >= args->data->time_to_die) && (args[i].eaten != 1))
			{
				pthread_mutex_lock(&args[i].data->stop_mutex);
				args->data->stop_flag = 1;
				pthread_mutex_unlock(&args[i].data->stop_mutex);
				pthread_mutex_unlock(&args[i].data_mutex);
				pthread_exit((void *)&args[i].id);
			}
			pthread_mutex_unlock(&args[i].data_mutex);
			i ++;
		}
	}
}

void	*test(void *arg)
{
	t_philo *args;
	int		stop;

	args =(t_philo *)arg;
	if ((args->id % 2) == 0)
		usleep(50);
	pthread_mutex_lock(&args->data_mutex);
	args->last_update = get_time();
	pthread_mutex_unlock(&args->data_mutex);
	while (1)
	{
		eat(args);
		pthread_mutex_lock(&args->data->stop_mutex);
		stop = args->data->stop_flag;
		pthread_mutex_unlock(&args->data->stop_mutex);
		if(stop != 1)
		{
			print_mute(args, 2, 0);
			print_mute(args, 3, 0);
			usleep(args->data->time_to_sleep);
		}
		else
			pthread_exit(NULL);
	}
	pthread_exit(NULL);
}

void create_philo(pthread_t *philo, t_philo *args)
{
	int			i = 0;
	pthread_t	monitoring;
	int			*dead;

	while (i < args->data->nb_philo)
	{
		if (pthread_create(&philo[i], NULL, test, &args[i]) != 0)
		{
			printf("Erreur lors de la création du thread %d\n", i + 1);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	if (pthread_create(&monitoring, NULL, monitor, args) != 0)
		{
			printf("Erreur lors de la création du thread %d\n", i + 1);
			exit(EXIT_FAILURE);
		}
	pthread_join(monitoring, (void **)&dead);
	i = 0;
	while (i < args->data->nb_philo)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	printf("%ld %d died\n", get_time(), *dead);
}

int	init_philo(t_philo *args, pthread_mutex_t *fork, t_data *data)
{
	int i;

	i = 0;
	while(i < data->nb_philo)
	{
		args[i].data = data;
		args[i].id = i + 1;
		args[i].share_count = 0;
		args[i].eaten = 0;
		args[i].last_update = get_time();
		args[i].r_fork = &fork[i];
		args[i].l_fork = &fork[(i + 1) % data->nb_philo];
		pthread_mutex_init(&args[i].data_mutex, NULL);
		i ++;
	}
	return(0);
}

int main(int c, char **v) 
{
	t_data			data;
	pthread_mutex_t *fork;
	pthread_t 		*philo;
	t_philo 		*args;
	int				i;

	if (c < 5 || c > 6)
		return 1;
	if (init_data(&data, v))
		return(0);
	if (data.nb_philo <= 0) {
		printf("Le nombre de philosophes doit être un entier positif.\n");
		return 1;
	}
	fork = fork_init(data.nb_philo);
	philo = malloc(sizeof(pthread_t) * data.nb_philo + 1);
	args = malloc(sizeof(t_philo) * data.nb_philo + 1);
	if (!philo || !args) {
	    perror("Erreur d'allocation de mémoire");
	    return 1;
	}
	if (init_philo(args, fork, &data) == 1)
	{
		printf("wrong argument\n");
		return(1);
	}
	create_philo(philo, args);
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
	return 0;
}

