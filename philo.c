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
	pthread_mutex_init(&reaper->phil[i].state.mutex, NULL);
	reaper->phil[i].fork1 = &reaper->forks[i];
	if (i == reaper->attendance - 1)
		reaper->phil[i].fork2 = &reaper->forks[0];
	else
		reaper->phil[i].fork2 = &reaper->forks[i + 1];
	reaper->phil[i].mutex = &reaper->mutex;
	reaper->phil[i].start_time = &reaper->start_time;
	reaper->phil[i].state.last_meal = 0;
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
		printf("%ld %d -------------------------DIED--------------------\n", curr_time - start_time, id);
	else if (action == FORK)
		printf("%ld %d picked up a fork\n", curr_time - start_time, id);
	else if (action == EATING)
		printf("%ld %d is eating\n", curr_time - start_time, id);
	else if (action == SLEEPING)
		printf("%ld %d is sleeping\n", curr_time - start_time, id);
	else if (action == THINKING)
		printf("%ld %d is thinking\n", curr_time - start_time, id);
}

int		phil_is_alive(t_phil *phil)
{
	int	res;

	res = 0;
	usleep(10);
	pthread_mutex_lock(&phil->state.mutex);
	if (phil->state.state == ALIVE)
		res = 1;
	pthread_mutex_unlock(&phil->state.mutex);
	return (res);
}

void	left_lunch_way(t_phil *phil)
{
	while (phil_is_alive(phil))
	{
		// printf("ID: %d state = %d\n", phil->id, phil->state);
		pthread_mutex_lock(phil->fork1);
		if (phil_is_alive(phil))
			print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork2);
		phil->state.last_meal = get_curr_time();
		if (phil_is_alive(phil))
			print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal) <= phil->eat
			&& phil_is_alive(phil))
			usleep(1);
		pthread_mutex_unlock(phil->fork2);
		pthread_mutex_unlock(phil->fork1);
		if (phil_is_alive(phil))
			print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal)
			<= (phil->eat + phil->sleep) && phil_is_alive(phil))
			usleep(1);
		if (phil_is_alive(phil))
			print_time(THINKING, *phil->start_time, phil->id);
	}
	// printf("ID: %d state = %d\n", phil->id, phil->state);
}

void	right_lunch_way(t_phil *phil)
{
	while (phil_is_alive(phil))
	{
		// printf("ID: %d state = %d\n", phil->id, phil->state);
		pthread_mutex_lock(phil->fork2);
		if (phil_is_alive(phil))
			print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork1);
		phil->state.last_meal = get_curr_time();
		if (phil_is_alive(phil))
			print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal) <= phil->eat
			&& phil_is_alive(phil))
			usleep(1);
		pthread_mutex_unlock(phil->fork1);
		pthread_mutex_unlock(phil->fork2);
		if (phil_is_alive(phil))
			print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal)
			<= (phil->eat + phil->sleep) && phil_is_alive(phil))
			usleep(1);
		if (phil_is_alive(phil))
			print_time(THINKING, *phil->start_time, phil->id);
	}
	// printf("ID: %d state = %d\n", phil->id, phil->state);
}

void	*routine(void *args)
{
	t_phil	*phil = (t_phil *)args;

	while(1)
	{
		pthread_mutex_lock(phil->mutex);
		phil->state.last_meal = *phil->start_time;
		pthread_mutex_unlock(phil->mutex);
		if (phil->state.last_meal)
			break ;
		usleep(10);
	}
	pthread_mutex_lock(&phil->state.mutex);
	phil->state.last_meal = get_curr_time();
	pthread_mutex_unlock(&phil->state.mutex);
	if (phil->id % 2 == 0)
		left_lunch_way(phil);
	else
		right_lunch_way(phil);
	return (0);
}


void	kill_philo(int *state, int id, t_reaper *reaper)
{
	pthread_mutex_lock(&reaper->phil[id - 1].state.mutex);
	*state = DEAD;
	pthread_mutex_unlock(&reaper->phil[id - 1].state.mutex);
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
		pthread_mutex_lock(&reaper->phil[i].state.mutex);
		if (reaper->phil[i].id != id)
			reaper->phil[i].state.state = DEAD;
		pthread_mutex_unlock(&reaper->phil[i].state.mutex);
	}
	i = -1;
	while (++i < reaper->attendance)
	{
		pthread_mutex_destroy(&reaper->forks[i]);
		pthread_join(reaper->phil[i].thread, NULL);
	}
}

void	hades_office(t_reaper *reaper)
{
	int		i;
	long	time;

	i = -1;
	usleep(1);
	while (++i < reaper->attendance)
	{
		pthread_mutex_lock(&reaper->phil[i].state.mutex);
		time = reaper->phil[i].state.last_meal;
		pthread_mutex_unlock(&reaper->phil[i].state.mutex);
		if (time && get_curr_time() - time >= reaper->hourglass)
		{
			kill_philo(&reaper->phil[i].state.state, reaper->phil[i].id, reaper);
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
		reaper.phil[i].state.state = ALIVE;
		reaper.phil[i].id = i + 1;
		setup_forks(&reaper, i);
		setup_timers(&reaper.phil[i], args);
		pthread_create(&reaper.phil[i].thread, NULL, routine, &reaper.phil[i]);
	}
	pthread_mutex_lock(&reaper.mutex);
	reaper.start_time = get_curr_time();
	pthread_mutex_unlock(&reaper.mutex);
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