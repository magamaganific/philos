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
	reaper->attendance = size;
	reaper->phil = (t_phil *)malloc(sizeof(t_phil) * size);
	reaper->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
	if (!reaper->phil || !reaper->attendance || !reaper->forks)
		return(0);
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

void	lunchtable(t_phil *phil)
{
	if (phil->id % 2 == 0)
	{
		pthread_mutex_lock(phil->fork2);
		phil->state = FORK;
	}
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
		usleep(100);
	}
	while (phil->state == ALIVE)
	{
		lunch_table(phil);
	}
	return (0);
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
	pthread_mutex_init(&reaper.mutex, NULL);
	while (++i < reaper.attendance)
	{
		reaper.phil[i].id = i + 1;
		setup_forks(&reaper, i);
		setup_timers(&reaper.phil[i], args);
		reaper.phil[i].mutex = &reaper.mutex;
		reaper.phil[i].start_time = &reaper.start_time;
		pthread_create(&reaper.phil[i].thread, NULL, routine, &reaper.phil[i]);
	}
	reaper.start_time = get_curr_time();
	printf("start time = %ld\n", reaper.start_time);
	usleep(400000);
	printf("end time = %ld\n", get_curr_time() - reaper.start_time);
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