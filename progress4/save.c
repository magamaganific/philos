
void	*routine(void *args)
{
	t_philosopher philo = *(t_philosopher *)args;
	if (*philo.start == 0)
		while (*philo.start == 0);
	philo.last_meal = *philo.start;
	if (!philo.last_meal)
		philo.last_meal = *philo.start;
	if (philo.state == ALIVE)
	{
		
	}
	return (NULL);
}


void	start_process(t_reaper *reaper)
{
	int	i = -1;

	while(++i < reaper->seats)
	{
		pthread_create(&reaper->subject[i].thread, NULL, routine, &reaper->subject[i]);
	}
	reaper->start = get_curr_time();
	printf("filosopher %d start: %ld\n", reaper->subject[0].id, *reaper->subject[0].start);
	hour_glass(reaper->subject[0]);
}