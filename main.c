#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include "main.h"

int	ft_ph_atoi(char *str)
{
	int num;
	int	i;

	num = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			num = num * 10 + (str[i] - '0');
		else
			return (-1);
	}
	return (num);
}

int	phil_initialize(t_program *philo, char **av)
{
	if ((ft_ph_atoi(av[1]) != -1 && ft_ph_atoi(av[1]) <= 200)
		&& ft_ph_atoi(av[2]) != -1 && ft_ph_atoi(av[3]) != -1
		&& ft_ph_atoi(av[4]) != -1)
	{
		philo->philo->philos_am = ft_ph_atoi(av[1]);
		philo->philo->death_t = ft_ph_atoi(av[2]);
		philo->philo->food_t = ft_ph_atoi(av[3]);
		philo->philo->bed_t = ft_ph_atoi(av[4]);
		philo->philo->eat_times = -1;
	}
	else
		return (0);
	if (av[5])
	{
		if (ft_ph_atoi(av[5]) != -1)
			philo->philo->eat_times  = ft_ph_atoi (av[5]);
		else
			return (0);
	}
	printf("hi\n");
	pthread_mutex_init(&philo->dead_CÐ, NULL);
	printf("hi3\n");
	pthread_mutex_init(&philo->meal_CÐ, NULL);
	pthread_mutex_init(&philo->write_CÐ, NULL);
	printf("hi\n");
	return (1);
}

void	phil_deestroy(t_program	*philo)
{
	pthread_mutex_destroy(&philo->dead_CÐ);
	pthread_mutex_destroy(&philo->meal_CÐ);
	pthread_mutex_destroy(&philo->write_CÐ);
}
int main(int ac, char **av)
{
	t_program philo;

	if (ac < 5 || ac < 6)
	{
		printf("invalid input\n");
		return (0);
	}
	philo.philo = malloc(sizeof(t_program));
	phil_initialize(&philo, av);
	printf("hi\n");
	phil_destroy(&philo);
	free(philo.philo);
}