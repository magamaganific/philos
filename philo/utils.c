/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:00:44 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/20 11:00:46 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (NULL);
	while (av[++i])
	{
		args[i - 1] = phatoi(av[i]);
		if (args[i - 1] == -1)
			return (free(args), NULL);
	}
	return (args);
}

long	get_curr_time(void)
{
	struct timeval	current;
	long			curr;

	gettimeofday(&current, NULL);
	curr = ((current.tv_sec * 1000) + (current.tv_usec / 1000));
	return (curr);
}

void	print_time(int action, long start_time, int id)
{
	long	curr_time;

	curr_time = get_curr_time();
	if (action == DEAD)
		printf("%ld %d died\n", curr_time - start_time, id);
	else if (action == FORK)
		printf("%ld %d picked up a fork\n", curr_time - start_time, id);
	else if (action == EATING)
		printf("%ld %d is eating\n", curr_time - start_time, id);
	else if (action == SLEEPING)
		printf("%ld %d is sleeping\n", curr_time - start_time, id);
	else if (action == THINKING)
		printf("%ld %d is thinking\n", curr_time - start_time, id);
}
