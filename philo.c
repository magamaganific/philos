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

void	init_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	reaper->seats = args[0];
	reaper->start = 0;
	while (++re < args[0])
	{
		reaper->subject[re].id = re;
		reaper->subject[re].hourglass = args[1];
		reaper->subject[re].eat = args[2];
		reaper->subject[re].rest = args[3];
		reaper->subject[re].last_meal = -1;
		pthread_mutex_init(&reaper->subject[re].mutex, NULL);
		pthread_mutex_init(&reaper->fork[re], NULL);
	}
}



void	start_process(t_reaper *reaper)
{
	int	i = -1;

	while(++i < reaper->seats)
	{
		reaper->subject[i].state = ALIVE;
		reaper->subject[i].start = &reaper->start;
		reaper->subject[i].fork1 = &reaper->fork[reaper->subject[i].id];
		if (i == reaper->seats - 1)
			reaper->subject[i].fork2 = &reaper->fork[0];
		else
			reaper->subject[i].fork2 = &reaper->fork[reaper->subject[i].id + 1];
		pthread_create(&reaper->subject[i].thread, NULL, routine, &reaper->subject[i]);
		pthread_mutex_init(&reaper->fork[i], NULL);
	}
	reaper->start = get_curr_time();
}

int main(void)
{
	t_reaper	reaper;
	int			argv[5] = {5, 600, 200, 700};
	init_philos(&reaper, argv);
	start_process(&reaper);
	destr_philos(&reaper, argv);
}