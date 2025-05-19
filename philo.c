#include "philo.h"

void	*routine(void *arg)
{
	t_kill *kill = (t_kill *)arg;
	int		start = 0;

	while (1)
	{
		pthread_mutex_lock(kill->start);
		start = *kill->flag;
		pthread_mutex_unlock(kill->start);
		if (start)
			break ;
		usleep(10);
	}
	while (2)
	{
		if (*kill->state == DEAD)
		{
			break ;
		}
		usleep(10);
	}
	printf("successful kill\n");
	return (0);
}

void	kill_a_kill(t_scythe *reaper)
{
	int i = -1;

	while (++i < 3)
	{
		pthread_mutex_lock(&reaper->test[i].kill);
		printf("killing: %d\n", i);
		pthread_mutex_unlock(&reaper->test[i].kill);
	}
}

int main(void)
{
	t_scythe	reaper;
	int			i = -1;

	pthread_mutex_init(&reaper.start, NULL);
	reaper.flag = 0;
	reaper.state = ALIVE;
	while (++i < 3)
	{
		reaper.test[i].start = &reaper.start;
		reaper.test[i].flag = &reaper.flag;
		pthread_mutex_init(&reaper.test[i].kill, NULL);
		reaper.kill[i] = reaper.test[i].kill;
		reaper.test[i].state = &reaper.state;
		pthread_create(&reaper.test[i].thread, NULL, routine, &reaper.test[i]);
	}
	pthread_mutex_lock(&reaper.start);
	reaper.flag = 1;
	pthread_mutex_unlock(&reaper.start);
	kill_a_kill(&reaper);
	i = -1;
	while (++i < 3)
	{
		pthread_join(reaper.test[i].thread, NULL);
		pthread_mutex_destroy(&reaper.test[i].kill);
	}
}