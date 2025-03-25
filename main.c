#include "philo.h"

void	*monintoring(void *arg)
{
	t_philo 		*args;
	struct timeval	now;
	long			elapse;

	args = (t_philo *)arg;
	while(1)
	{
		gettimeofday(&now, NULL);
		elapse = (now.tv_sec - args->last_update->tv_sec) * 1000 +
				(now.tv_usec - args->last_update->tv_usec) / 1000;
		if (elapse > args->data.time_to_die)
			
	}
}
void	*test(void *arg)
{
	t_philo *args;

	args =(t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(args->mutex);
		*(args->share_count) += 1;
		gettimeofday(args->last_update, NULL);
		printf("thread %d compteur :%d\n",args->id, *(args->share_count));
		pthread_mutex_unlock(args->mutex);
		usleep(10);
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
void create_philo(pthread_t *philo, t_philo *args, int num_philo, pthread_mutex_t *mutex) {
    int i = 0;
	int share_count = 0;

	while (i < num_philo) {
		args[i].id = i + 1;
		args[i].share_count = &share_count;
		args[i].mutex = mutex;
		if (pthread_create(&philo[i], NULL, test, &args[i]) != 0) {
			printf("Erreur lors de la création du thread %d\n", i + 1);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	usleep(10000);
	stop_thread(philo, num_philo);

	i = 0;
	while (i < num_philo) {
		pthread_join(philo[i], NULL);
		i++;
	}
	printf("end\n");
}

int main(int c, char **v) {
	int num_philo;
	pthread_mutex_t mutex;

	if (c != 2)
		return 1;
	num_philo = atoi(v[1]);
	if (num_philo <= 0) {
		printf("Le nombre de philosophes doit être un entier positif.\n");
		return 1;
	}
	pthread_t *philo = malloc(sizeof(pthread_t) * num_philo);
	t_philo *args = malloc(sizeof(t_philo) * num_philo);
	if (!philo || !args) {
	    perror("Erreur d'allocation de mémoire");
	    return 1;
	}	
	pthread_mutex_init(&mutex, NULL);
	create_philo(philo, args, num_philo, &mutex);
	pthread_mutex_destroy(&mutex);
	free(philo);
	free(args);
	return 0;
}