#include "philo.h"

void	*monitor(void *arg)
{
	t_philo 		*args;
	struct timeval	now;
	long			elapse;
	int				i;
	int				*dead_philo;

	args = (t_philo *)arg;
	dead_philo = malloc(sizeof(int));
	while(1)
	{
		i = 0;
		gettimeofday(&now, NULL);
		while (i < args->data.nb_philo)
		{
			elapse = (now.tv_sec - args[i].last_update->tv_sec) * 1000 +
			(now.tv_usec - args[i].last_update->tv_usec) / 1000;
			if (elapse >= args->data.time_to_die)
			{
				*(args->stop_flag) = 1;
				*dead_philo = args[i].id;
				pthread_exit((void *)dead_philo);
			}
			i ++;
		}
		usleep(50);
	}
}

void	*test(void *arg)
{
	t_philo *args;

	args =(t_philo *)arg;
	gettimeofday(args->last_update, NULL);
	while (1)
	{
		pthread_mutex_lock(args->mutex);
		*(args->share_count) += 1;
		usleep(100);
		pthread_mutex_unlock(args->mutex);
		printf("thread %d eat\n",args->id);
		gettimeofday(args->last_update, NULL);
		usleep(100);
	}
	pthread_exit(NULL);
}

void	stop_thread(pthread_t* philo, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		pthread_cancel(philo[i]);
		i ++;
	}
}
void create_philo(pthread_t *philo, t_philo *args)
{
	int			i = 0;
	int			stop_flag = 0;
	pthread_t	monitoring;
	int			*dead;

	while (i < args->data.nb_philo)
	{
		args[i].stop_flag = &stop_flag;
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
		pthread_detach(philo[i]);
		i++;
	}
	printf("philosophe %d is dead\n", *dead);
}

void init_philo(t_philo *args, int time_die, pthread_mutex_t *mutex, int nb_philo)
{
	int i;
	int share_count;

	i = 0;
	share_count = 0;
	while(i < nb_philo)
	{
		args[i].id = i + 1;
		args[i].share_count = &share_count;
		args[i].mutex = mutex;
		args[i].data.time_to_die = time_die;
		args[i].last_update = malloc(sizeof(struct timeval));
		gettimeofday(args[i].last_update, NULL);
		args[i].data.nb_philo = nb_philo;
		i ++;
	}
}

int main(int c, char **v) 
{
	int num_philo;
	pthread_mutex_t mutex;

	if (c != 3)
		return 1;
	num_philo = atoi(v[1]);
	if (num_philo <= 0) {
		printf("Le nombre de philosophes doit être un entier positif.\n");
		return 1;
	}
	pthread_t *philo = malloc(sizeof(pthread_t) * num_philo + 1);
	t_philo *args = malloc(sizeof(t_philo) * num_philo + 1);
	if (!philo || !args) {
	    perror("Erreur d'allocation de mémoire");
	    return 1;
	}
	pthread_mutex_init(&mutex, NULL);
	init_philo(args, atoi(v[2]), &mutex, num_philo);
	create_philo(philo, args);
	pthread_mutex_destroy(&mutex);
	free(philo);
	free(args);
	return 0;
}
