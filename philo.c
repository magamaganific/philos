#include "philo.h"

long	get_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec);
}

long	timestamp(long start, long end)
{
	return(end - start);
}

void	*routine(void *args)
{
	p_hilosopher	*philos = (p_hilosopher *)args;

	while (*philos->r_start == 0);
	philos->last_meal = *philos->r_start;
	while (philos->state == ALIVE)
	{
		philos->repeat++;
		pthread_mutex_lock(philos->fork1);
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d picked up a fork", philos->id);
		pthread_mutex_lock(philos->fork2);
		philos->last_meal = get_curr_time();
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d is eating", philos->id);
		while (timestamp(philos->last_meal, get_curr_time()) <= philos->eat)
			usleep(10);
		while (timestamp(philos->last_meal, get_curr_time()) <= philos->eat + philos->sleep);
	}
	printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
	printf("Philo %d DIED", philos->id);
	return(NULL);
}

void	ft_lock_all(p_urgatory *table)
{
	int	i = -1;

	while(table->subject[++i].id)
		pthread_mutex_lock(&table->subject[i].reap);
}

void	*monitoring(void *args)
{
	grim_r		*reaper = (grim_r *)args;
	p_urgatory	*table = &reaper->table;
	int		i = -1;

	reaper->r_start = get_curr_time();
	while (table->subject[++i].id)
	{
		if (table->subject[i].last_meal >= reaper->hourglass)
		{
			table->subject[i].state = DEAD;
			ft_lock_all(table);
			break ;
		}
		else if (reaper->rep != -1 && table->subject[i].repeat == reaper->rep)
			pthread_mutex_lock(&table->subject[i].reap);
	}
	return(NULL);
}

void	preparations(grim_r *reaper, int *args)
{
	int			i = -1;
	p_urgatory *table = &reaper->table;

	reaper->r_start = 0;
	table->subjects = args[0];
	reaper->hourglass = args[1];
	reaper->end = &table->death;
	reaper->rep = args[4];
	while (++i < table->subjects)
	{
		table->subject[i].id = i + 1;
		table->subject[i].eat = args[2];
		table->subject[i].sleep = args[3];
		table->subject[i].state = ALIVE;
		pthread_mutex_init(&table->subject[i].reap, NULL);
		pthread_mutex_init(&table->fork[i], NULL);
		table->subject[i].fork1 = &table->fork[i];
		if (i == table->subjects -1)
			table->subject[i].fork2 = &table->fork[0];
		table->subject[i].fork2 = &table->fork[i + 1];
		table->subject[i].r_start = &reaper->r_start;
		pthread_create(&table->subject[i].thread, NULL, routine, &table->subject[i]);
	}
	pthread_create(&reaper->leash, NULL, monitoring, &reaper);
	
}

void	finish(grim_r *reaper)
{
	p_urgatory	*table = &reaper->table;
	int	i = -1;
	
	while (table->subject[++i].id)
	{
		pthread_unlock
		pthread_join(table->subject[i].thread, NULL);
	}
}

int	main(void)
{
	int		args[5] = {4, 600, 200, 300, 10};
	grim_r	reaper;

	preparations(&reaper, args);
	finish(&reaper);
}