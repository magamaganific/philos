#include "philo.h"

void	destr_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	while (++re < args[0])
	{
		pthread_mutex_destroy(&reaper->fork[re]);
		pthread_join(&reaper->subject[re].thread, NULL);
	}

}

void	init_philos(t_reaper *reaper, int *args)
{
	int		re = -1;

	reaper->hourglass=args[1];
	while (++re < args[0])
	{
		reaper->subject[re].id = re;
		reaper->subject[re].eat = args[2];
		reaper->subject[re].rest = args[3];
		pthread_mutex_init(&reaper->fork[re], NULL);
	}
}

void	start_process(t_reaper *reaper)
{
	int	i = -1;
	while(++i <= 10)
	{

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