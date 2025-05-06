#include "philo.h"

void	destr_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	while (++re < args[0])
	{
		pthread_mutex_destroy(&reaper->fork[re]);
		pthread_join(reaper->subject[re].thread, NULL);
	}

}

void	init_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	reaper->hourglass=args[1];
	reaper->seats=args[0];
	while (++re < args[0])
	{
		reaper->subject[re].id = re;
		reaper->subject[re].eat = args[2];
		reaper->subject[re].rest = args[3];
		pthread_mutex_init(&reaper->fork[re], NULL);
	}
}

void	*routine(void *args)
{
	t_philosopher philo = *(t_philosopher *)args;

	pthread_mutex_lock(philo.fork1);
	printf("philosopher %d took fork1\n", philo.id);
	pthread_mutex_lock(philo.fork2);
	printf("philosopher %d is eating\n", philo.id);
	pthread_mutex_unlock(philo.fork1);
	pthread_mutex_unlock(philo.fork2);
	return (NULL);
}

void	start_process(t_reaper *reaper)
{
	int	i = -1;

	while(++i < reaper->seats)
	{
		reaper->subject[i].fork1 = &reaper->fork[reaper->subject[i].id];
		if (i == reaper->seats - 1)
			reaper->subject[i].fork2 = &reaper->fork[0];
		else
			reaper->subject[i].fork2 = &reaper->fork[reaper->subject[i].id + 1];
		pthread_create(&reaper->subject[i].thread, NULL, routine, &reaper->subject[i]);
		pthread_mutex_init(&reaper->fork[i], NULL);
	}
}

int main(void)
{
	t_reaper	reaper;
	int			argv[5] = {4, 600, 200, 200};
	init_philos(&reaper, argv);
	start_process(&reaper);
	destr_philos(&reaper, argv);
}