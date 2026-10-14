/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:52:23 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/12 11:52:26 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_time(char *label, int id)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	printf("ID: %d, ACTION: %s (%ld.%06ld)\n",  id, label, tv.tv_sec, tv.tv_usec);
}

void	pherror(void)
{
	printf("Invalid input\n");
}

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
