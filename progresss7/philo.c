#include "philo.h"


int	ft_is_allnum(char *av)
{
	int	i;

	i = -1;
	while (av[++i])
	{
		if (!(av[i] <= '9' && av[i] >= '0'))
			return (0);
	}
	return (1);
}

int	phatoi(char *av)
{
	long	num;
	int		i;

	num = 0;
	i = -1;
	if (ft_is_allnum(av))
	{
		while (av[++i])
		{
			num = num * 10 + av[i] - '0';
		}
		if (num <= __INT_MAX__)
			return (num);
	}
	return (-1);
}

int	*get_args(char **av, int size)
{
	int	*args;
	int	i;

	i = 0;
	args = (int *)malloc(sizeof(int) * size);
	if (!args)
		return(NULL);
	while (av[++i])
	{
		args[i - 1] = phatoi(av[i]);
		if (args[i - 1] == -1)
			return(free(args), NULL);
	}
	return (args);
}

int	init_reaper(t_reaper *reaper, int size)
{
	reaper->start_time = 0;
	reaper->attendance = size;
	reaper->phil = (t_phil *)malloc(sizeof(t_phil) * size);
	reaper->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
	pthread_mutex_init(&reaper->mutex, NULL);
	if (!reaper->phil || !reaper->attendance || !reaper->forks)
	{
		pthread_mutex_destroy(&reaper->mutex);
		return(0);
	}
	return(1);
}

void	setup_forks(t_reaper *reaper, int i)
{
	pthread_mutex_init(&reaper->forks[i], NULL);
	reaper->phil[i].fork1 = &reaper->forks[i];
	if (i == reaper->attendance - 1)
		reaper->phil[i].fork2 = &reaper->forks[0];
	else
		reaper->phil[i].fork2 = &reaper->forks[i + 1];
}

void	setup_timers(t_phil	*phil, int *args)
{
	phil->eat = args[2];
	phil->sleep = args[3];
	phil->think = args[1] - (args[2] - args[3]);
}


long	get_curr_time(void)
{
	struct timeval	current;
	gettimeofday(&current, NULL);
	long curr = ((current.tv_sec * 1000) + (current.tv_usec / 1000));
	return (curr);
}

void	print_time(int action, long start_time, int id)
{
	long	curr_time;

	curr_time = get_curr_time();
	if (action == DEAD)
		printf("%ld philo %d DIED\n", curr_time - start_time, id);
	else if (action == FORK)
		printf("%ld philo %d picked up a fork\n", curr_time - start_time, id);
	else if (action == EATING)
		printf("%ld philo %d is eating\n", curr_time - start_time, id);
	else if (action == SLEEPING)
		printf("%ld philo %d is sleeping\n", curr_time - start_time, id);
}

void	left_lunch_way(t_phil *phil)
{
	while (phil->state == ALIVE)
	{
		// printf("ID: %d state = %d\n", phil->id, phil->state);
		pthread_mutex_lock(phil->fork1);
		phil->action = FORK;
		print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork2);
		phil->action = EATING;
		phil->last_meal = get_curr_time();
		print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->last_meal) <= phil->eat)
			usleep(10);
		pthread_mutex_unlock(phil->fork2);
		pthread_mutex_unlock(phil->fork1);
		phil->action = SLEEPING;
		print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->last_meal) <= (phil->eat + phil->sleep))
			usleep(10);
	}
	// printf("ID: %d state = %d\n", phil->id, phil->state);
}

void	right_lunch_way(t_phil *phil)
{
	usleep(10);
	while (phil->state == ALIVE)
	{
		// printf("ID: %d state = %d\n", phil->id, phil->state);
		pthread_mutex_lock(phil->fork2);
		phil->action = FORK;
		print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork1);
		phil->action = EATING;
		phil->last_meal = get_curr_time();
		print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->last_meal) < phil->eat)
			usleep(10);
		pthread_mutex_unlock(phil->fork1);
		pthread_mutex_unlock(phil->fork2);
		phil->action = SLEEPING;
		print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->last_meal) < (phil->eat + phil->sleep))
			usleep(10);
		phil->action = THINKING;
	}
	// printf("ID: %d state = %d\n", phil->id, phil->state);
}

void	*routine(void *args)
{
	t_phil	*phil = (t_phil *)args;

	while(1)
	{
		pthread_mutex_lock(phil->mutex);
		phil->last_meal = *phil->start_time;
		pthread_mutex_unlock(phil->mutex);
		if (phil->last_meal)
			break ;
		usleep(10);
	}
	phil->last_meal = get_curr_time();
	usleep(10);
	if (phil->id % 2 == 0)
		left_lunch_way(phil);
	else
		right_lunch_way(phil);
	return (0);
}


void	kill_philo(int *state, int id, t_reaper *reaper)
{
	*state = DEAD;
	print_time(DEAD, reaper->start_time, id);
}

void	phil_stop(int *state)
{
	*state = DEAD;
}

void	stop_philo(t_reaper *reaper, int id)
{
	int	i;

	i = -1;
	while (++i < reaper->attendance)
	{
		if (reaper->phil[i].id != id)
			reaper->phil[i].state = DEAD;
		printf("stopping: %d\n", reaper->phil[i].id);
	}
	i = -1;
	while (++i < reaper->attendance)
	{
		printf("2nd stopping: %d\n", reaper->phil[i].id);
		pthread_join(reaper->phil[i].thread, NULL);
		pthread_mutex_destroy(&reaper->forks[i]);
	}
}

void	hades_office(t_reaper *reaper)
{
	int	i;

	i = -1;
	usleep(20);
	while (++i < reaper->attendance)
	{
		if ((get_curr_time() - reaper->phil[i].last_meal) >= reaper->hourglass)
		{
			kill_philo(&reaper->phil[i].state, reaper->phil[i].id, reaper);
			stop_philo(reaper, reaper->phil[i].id);
			break ;
		}
		if (i == reaper->attendance - 1)
			i = -1;
	}
}

void	end_hades(t_reaper *reaper, int *args)
{
	free(args);
	free(reaper->phil);
	free(reaper->forks);
}

int	til_death_phil(char **av)
{
	t_reaper reaper;
	int		*args;
	int		i;

	i = -1;
	args = get_args(av, 4);
	if (!args)
		return(0);
	if (!init_reaper(&reaper, args[0]))
		return(0);
	reaper.hourglass = args[1];
	while (++i < reaper.attendance)
	{
		reaper.phil[i].state = ALIVE;
		reaper.phil[i].id = i + 1;
		setup_forks(&reaper, i);
		setup_timers(&reaper.phil[i], args);
		reaper.phil[i].mutex = &reaper.mutex;
		reaper.phil[i].start_time = &reaper.start_time;
		pthread_create(&reaper.phil[i].thread, NULL, routine, &reaper.phil[i]);
	}
	reaper.start_time = get_curr_time();
	hades_office(&reaper);
	end_hades(&reaper, args);
	return (1);
}

int	main(int ac, char **av)
{
	if (ac == 5)
	{
		if (!til_death_phil(av))
			printf("invalid, input\n");
	}
	else if (ac == 6)
	{
		// if (!repeat_phil(av))
			printf("invalid, input\n");
	}
	else
		printf("invalid, input\n");
	return (0);
}


	// printf("start time = %ld\n", reaper.start_time);
	// usleep(400000);
	// printf("end time = %ld\n", get_curr_time() - reaper.start_time);