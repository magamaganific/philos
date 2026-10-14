#include "philo.h"

void	destroy_table(t_table *table)
{
	int rep = -1;
	int philos = table->phil_num;

	while(++rep < philos)
	{
		pthread_mutex_destroy(&forks[rep]);
	}

}

void	init_table(t_table *table, int philos, int countdown, int eat, int sleep)
{
	int	rep = -1;

	table->phil_num = philos;
	while (++rep < philos)
	{
		table->philos[rep].id = rep;
		pthread_mutex_init(&table->philos[rep].mutex, NULL);
		table->philos[rep].countdown = countdown;
		table->philos[rep].eating = eat;
		table->philos[rep].sleeping	= sleep;
		table->philos[rep].is_dead = 0;
		pthread_mutex_init(&forks[rep], NULL);
	}
}


void	*routine(void *arg)
{
	t_philo 		phil = *(t_philo *)arg;
	int				id = phil.id;
	int				l_fork = phil.l_fork;
	int				r_fork = phil.r_fork;
	
	if (l_fork < r_fork)
	{
		pthread_mutex_lock(&forks[l_fork]);
		pthread_mutex_lock(&forks[r_fork]);
	}
	else
	{
		pthread_mutex_lock(&forks[r_fork]);
		pthread_mutex_lock(&forks[l_fork]);
	}
    printf("Thread %d accessing forks %d and %d\n", id + 1, l_fork + 1, r_fork + 1);
	pthread_mutex_unlock(&forks[r_fork]);
	pthread_mutex_unlock(&forks[l_fork]);
	return (NULL);
}

void	start_program(t_table *table)
{
	int	rep = -1;
	int philos = table->phil_num;

	while (++rep < philos)
	{
		table->philos[rep].l_fork = rep;
		if(rep == philos - 1)
			table->philos[rep].r_fork = 0;
		else
			table->philos[rep].r_fork = rep + 1;
		pthread_create(&table->philos[rep].thread, NULL, routine, &table->philos[rep]);
	}
}

void	end_program(t_table *table)
{
	int	rep = -1;
	int philos = table->phil_num;

	while (++rep < philos)
	{
		pthread_join(table->philos[rep].thread, NULL);
	}
}

int	main(void)
{
	int		philos = 4;
	int		countdown = 500;
	int 	eat = 200;
	int 	sleep = 200;
	t_table table;
	
	init_table(&table, philos, countdown, eat, sleep);
	start_program(&table);
	end_program(&table);
	destroy_table(&table);
}