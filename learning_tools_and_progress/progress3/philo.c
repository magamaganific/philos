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

	if (philos->last_meal == 0)
	{
		printf("routine\n");
		while (*philos->r_start == 0);
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("routine start\n");
		philos->last_meal = *philos->r_start;
		if (philos->id % 2 == 0)
			usleep(10);
	}
	if (philos->state == ALIVE)
	{
		philos->repeat++;
		pthread_mutex_lock(philos->fork1);
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d picked up a fork\n", philos->id);
		pthread_mutex_lock(philos->fork2);
		philos->last_meal = get_curr_time();
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d is eating\n", philos->id);
		while (timestamp(philos->last_meal, get_curr_time()) <= philos->eat)
			usleep(10);
		pthread_mutex_unlock(philos->fork1);
		pthread_mutex_unlock(philos->fork2);
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d is sleeping\n", philos->id);
		while (timestamp(philos->last_meal, get_curr_time()) <= philos->eat + philos->sleep)
			usleep(10);
	}
	if (philos->state == DEAD)
	{
		printf("Time: %ld - ", timestamp(*philos->r_start, get_curr_time()));
		printf("Philo %d DIED\n", philos->id);
	}
	return(NULL);
}

void	ft_lock_all(p_urgatory *table)
{
	int	i = -1;

	while(table->subject[++i].id)
	{
		if(table->subject[i].state != LOCKED)
		{
			pthread_mutex_lock(&table->subject[i].reap);
			table->subject[i].state = LOCKED;
		}
	}
}

void	*monitoring(void *args)
{
	grim_r		*reaper = (grim_r *)args;
	p_urgatory	*table = &reaper->table;
	int		i = -1;

	while (reaper->r_start == 0);
	while (++i < table->subjects)
	{
		if (table->subject[i].last_meal >= reaper->hourglass)
		{
			table->subject[i].state = DEAD;
			pthread_mutex_lock(&table->death);
			ft_lock_all(table);
			pthread_mutex_unlock(&table->death);
			break ;
		}
		else if (reaper->rep != -1 && table->subject[i].repeat == reaper->rep)
			if (table->subject[i].state != LOCKED)
			{
				pthread_mutex_lock(&table->subject[i].reap);
				table->subject[i].state = LOCKED;
			}
		if (i == table->subjects - 1)
			i = -1;
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
	pthread_create(&reaper->leash, NULL, monitoring, &reaper);
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
	reaper->r_start = get_curr_time();
}

void	finish(grim_r *reaper)
{
	p_urgatory	*table = &reaper->table;
	int	i = -1;
	
	while (++i < table->subjects)
	{
		if (table->subject[i].state == LOCKED)
			pthread_mutex_unlock(&table->subject[i].reap);
		pthread_join(table->subject[i].thread, NULL);
	}
}

int	main(void)
{
	int		args[5] = {4, 600, 10, 10, 10};
	grim_r	reaper;

	preparations(&reaper, args);
	finish(&reaper);
	pthread_join(reaper.leash, NULL);
}