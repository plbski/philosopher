#include "philo.h"

void	*test(void *arg)
{
	t_philo *args = (t_philo *)arg;
	while (*(args->share_count) < 20)
	{
		pthread_mutex_lock(args->mutex);
		*(args->share_count) += 1;
		printf("thread %d compteur :%d\n",args->id, *(args->share_count));
		pthread_mutex_unlock(args->mutex);
		usleep(10);
	}
	pthread_exit(NULL);
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