#include "philo.h"

void	*monitor(void *arg)
{
	t_philo 		*args;
	unsigned int			elapse;
	int				i;
	int				*dead_philo;
	unsigned int				t;

	args = (t_philo *)arg;
	dead_philo = malloc(sizeof(int));
	while(1)
	{
		usleep(500);
		i = 0;
		t = get_time();
		while (i < args->data.nb_philo)
		{
			t = get_time();
			elapse = t - args[i].last_update;
			if ((elapse >= args->data.time_to_die) && (args[i].eaten != 1))
			{
				*(args->stop_flag) = 1;
				*dead_philo = args[i].id;
				pthread_exit((void *)dead_philo);
			}
			i ++;
		}
		elapse = 0;
	}
}

void	*test(void *arg)
{
	t_philo *args;

	args =(t_philo *)arg;
	if ((args->id % 2) == 0)
		usleep(50);
	args->last_update = get_time();
	while (1)
	{
		pthread_mutex_lock(args->r_fork);
		pthread_mutex_lock(args->l_fork);
		args->eaten = 1;
		printf("philo %d eat\n",args->id);
		usleep(args->data.time_to_eat);
		args->last_update = get_time();
		pthread_mutex_unlock(args->r_fork);
		pthread_mutex_unlock(args->l_fork);
		args->eaten = 0;
		args->share_count ++;
		printf("philo %d think\n", args->id);
		printf("philo %d dodo\n", args->id);
		usleep(args->data.time_to_sleep);
	}
	pthread_exit(NULL);
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

void init_philo(t_philo *args, char **v, pthread_mutex_t *fork, int nb_philo)
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
		i ++;
	}
	printf("mort : %d, mange : %d, sleep : %d\n", args[i -1].data.time_to_die, args[i -1].data.time_to_eat, args[i -1].data.time_to_sleep);
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
	init_philo(args, v, fork, num_philo);
	create_philo(philo, args);
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_destroy(&fork[i]);
		i ++;
	}
	free(philo);
	free(args);
	return 0;
}
