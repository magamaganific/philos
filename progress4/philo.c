#include "philo.h"


void	print_time(char *label, int id)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	printf("(%ld.%06ld) ID: %d %s\n", tv.tv_sec, tv.tv_usec,  id, label);
}

void	destr_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	while (++re < args[0])
	{
		pthread_mutex_destroy(&reaper->fork[re]);
		pthread_join(reaper->subject[re].thread, NULL);
	}
	pthread_mutex_destroy(&reaper->mutex);
}

long	get_curr_time(void)
{
	struct timeval	current;
	gettimeofday(&current, NULL);
	long curr = ((current.tv_sec * 1000) + current.tv_usec);
	return (curr);
}

long	hour_glass(long start, long end)
{
	return( end - start);
}

void	*limbo(void	*args)
{
	t_philosopher *philo = (t_philosopher *) args;

	while (*philo->start == 0)
	{
		pthread_mutex_lock(philo->mutex);
		philo->last_meal = *philo->start;
		pthread_mutex_unlock(philo->mutex);
		if (philo->id % 2 == 0)
			usleep(10);
		if (philo->last_meal)
			break ;
		usleep(100);
	}
	print_time("START", philo->id);
	usleep(10);
	while (philo->state == ALIVE)
	{
		if (hour_glass(philo->last_meal, get_curr_time()) >= philo->hourglass)
			philo->state = DEAD;
		pthread_mutex_lock(philo->fork1);
		print_time("picked up a fork", philo->id);
		pthread_mutex_lock(philo->fork2);
		philo->last_meal = get_curr_time();
		print_time("is eating", philo->id);
		usleep(200);
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		usleep(200);
	}
	if (philo->state == DEAD)
		print_time("DIED", philo->id);
	return(NULL);
}

void	init_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	reaper->seats = args[0];
	reaper->start = 0;
	pthread_mutex_init(&reaper->mutex, NULL);
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
		reaper->subject[re].start = &reaper->start;
		reaper->subject[re].mutex = &reaper->mutex;
		pthread_mutex_init(&reaper->fork[re], NULL);
		pthread_create(&reaper->subject[re].thread, NULL, limbo, &reaper->subject[re]);
	}
	reaper->start = get_curr_time();
}

int main(void)
{
	t_reaper	reaper;
	int			argv[5] = {5, 900, 200, 200};
	
	init_philos(&reaper, argv);
	// start_process(&reaper);
	destr_philos(&reaper, argv);
}