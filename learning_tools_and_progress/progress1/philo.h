#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/time.h>

pthread_mutex_t forks[250];

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	mutex;
	int				id;
	size_t			countdown;
	size_t			time_start;
	int				l_fork;
	int				r_fork;
	size_t			eating;
	size_t			sleeping;
	size_t			is_dead;
}	t_philo;

typedef struct s_table
{
	int		phil_num;
	t_philo	philos[250];
}	t_table;

typedef struct s_reaper
{
	t_table			table;
	pthread_mutex_t	*philo;
}	t_reaper;