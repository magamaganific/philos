#include "philo.h"

void	destr_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	while (++re < args[0])
	{
		pthread_mutex_destroy(&reaper->fork[re]);
		pthread_join(reaper->subject[re].thread, NULL);
		pthread_mutex_destroy(&reaper->subject[re].mutex);
	}
}

void	init_philos(t_reaper *reaper, pthread_t *setup, int *args)
{
	int		re = -1;

	reaper->seats = args[0];
	reaper->start = 0;
	while (++re < args[0])
	{
		reaper->subject[re].id = re + 1;
		reaper->subject[re].state = ALIVE;
		reaper->subject[re].hourglass = args[1];
		reaper->subject[re].eat = args[2];
		reaper->subject[re].rest = args[3];
		reaper->subject[re].last_meal = 0;
		reaper->subject[re].fork1 = &reaper->fork[re];
		if (re == reaper->seats - 1)
			reaper->subject[re].fork2 = &reaper->fork[0];
		else
			reaper->subject[re].fork2 = &reaper->fork[re + 1];
		reaper->subject[re].setup = setup;
		reaper->subject[re].start = &reaper->start;
		pthread_mutex_init(&reaper->subject[re].mutex, NULL);
		pthread_mutex_init(&reaper->fork[re], NULL);
	}
}

long	get_curr_time(void)
{
	struct timeval	current;
	gettimeofday(&current, NULL);
	long curr = ((current.tv_sec * 1000) + current.tv_usec);

	printf("Attained time:%ld\n", curr);
	return (curr);
}

long	hour_glass(long start, long end)
{
	return( end - start);
}

void	*routine(void *args)
{
	t_philosopher	*philo = (t_philosopher *) args;
	while(*philo->start == 0);
	printf("setup finished! \n");
	printf("start %d: %ld\n", philo->id, *philo->start);
	usleep(200);
	printf("end %d: %ld\n", philo->id, hour_glass(*philo->start, get_curr_time()));
	return(NULL);
}

void	*underworld(void *args)
{
	t_reaper	*reaper = (t_reaper *)args;
	int			i = -1;

	while (++i < reaper->seats)
		pthread_create(&reaper->subject[i].thread, NULL, routine, &reaper->subject[i]);
	reaper->start = get_curr_time();
	return(NULL);
}

int main(void)
{
	t_reaper	reaper;
	pthread_t	setup;
	int			argv[5] = {5, 600, 200, 700};
	init_philos(&reaper, &setup, argv);
	pthread_create(&setup, NULL, underworld, &reaper);
	pthread_join(setup, NULL);
	// start_process(&reaper);
	destr_philos(&reaper, argv);
}