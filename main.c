#include "philo.h"

void	*monitor(void *arg)
{
	t_philo 					*args;
	unsigned int				elapse;
	int							i;
	unsigned int				t;

	args = (t_philo *)arg;
	while(1)
	{
		usleep(500);
		i = 0;
		t = get_time();
		while (i < args->data.nb_philo)
		{
			t = get_time();
			pthread_mutex_lock(&args[i].data_mutex);
			elapse = t - args[i].last_update;
			if ((elapse >= args->data.time_to_die) && (args[i].eaten != 1))
			{

				pthread_mutex_lock(args[i].stop_mutex);
				*(args->stop_flag) = 1;
				pthread_mutex_unlock(&args[i].data_mutex);
				pthread_mutex_unlock(args[i].stop_mutex);
				pthread_exit((void *)&args[i].id);
			}
			pthread_mutex_unlock(&args[i].data_mutex);
			i ++;
		}
		elapse = 0;
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
		manage_fork(arg, 0);
		pthread_mutex_lock(&args->data_mutex);
		args->eaten = 1;
		pthread_mutex_lock(args->stop_mutex);
		stop = *(args->stop_flag);
		if (stop!= 1)
			printf("philo %d eat\n",args->id);
		pthread_mutex_unlock(args->stop_mutex);
		pthread_mutex_unlock(&args->data_mutex);
		usleep(args->data.time_to_eat);
		pthread_mutex_lock(&args->data_mutex);
		args->last_update = get_time();
		args->eaten = 0;
		args->share_count ++;
		pthread_mutex_unlock(&args->data_mutex);
		manage_fork(arg, 1);
		if(*(args->stop_flag) != 1)
		{
			printf("philo %d think\n", args->id);
			printf("philo %d dodo\n", args->id);
			usleep(args->data.time_to_sleep);
		}
		else
			pthread_exit(NULL);
	}
	pthread_exit(NULL);
}

void create_philo(pthread_t *philo, t_philo *args)
{
	int			i = 0;
	int			*stop_flag;
	pthread_t	monitoring;
	int			*dead;
	pthread_mutex_t	*stop;

	stop_flag = malloc(sizeof(int));
	*stop_flag = 0;
	stop = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(stop, NULL);
	while (i < args->data.nb_philo)
	{
		args[i].stop_flag = stop_flag;
		args[i].stop_mutex = stop;
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
	while (i < args->data.nb_philo)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	pthread_mutex_destroy(stop);
	free(stop);
	free(stop_flag);
	printf("philosophe %d is dead\n", *dead);
}

int	init_philo(t_philo *args, char **v, pthread_mutex_t *fork, int nb_philo)
{
	int i;

	i = 0;
	while(i < nb_philo)
	{
		args[i].id = i + 1;
		args[i].share_count = 0;
		args[i].data.time_to_die = ft_atoi(v[2]);
		args[i].data.time_to_eat = ft_atoi(v[3]) * 1000;
		args[i].data.time_to_sleep = ft_atoi(v[4]) * 1000;
		args[i].eaten = 0;
		args[i].share_count = 0;
		args[i].last_update = get_time();
		args[i].data.nb_philo = nb_philo;
		args[i].r_fork = &fork[i];
		args[i].l_fork = &fork[(i + 1) % nb_philo];
		pthread_mutex_init(&args[i].data_mutex, NULL);
		if (args[i].data.time_to_die <= 0 || args[i].data.time_to_eat <= 0 || args[i].data.time_to_sleep <=0)
			return(1);
		i ++;
	}
	return(0);
}

int main(int c, char **v) 
{
	int 			num_philo;
	pthread_mutex_t *fork;
	pthread_t 		*philo;
	t_philo 		*args;
	int				i;

	if (c < 5 || c > 6)
		return 1;
	num_philo = atoi(v[1]);
	if (num_philo <= 0) {
		printf("Le nombre de philosophes doit être un entier positif.\n");
		return 1;
	}
	fork = fork_init(num_philo);
	philo = malloc(sizeof(pthread_t) * num_philo + 1);
	args = malloc(sizeof(t_philo) * num_philo + 1);
	if (!philo || !args) {
	    perror("Erreur d'allocation de mémoire");
	    return 1;
	}
	if (init_philo(args, v, fork, num_philo) == 1)
	{
		printf("wrong argument\n");
		return(1);
	}
	create_philo(philo, args);
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_destroy(&fork[i]);
		pthread_mutex_destroy(&args[i].data_mutex);
		i ++;
	}
	free(fork);
	free(philo);
	free(args);
	return 0;
}

