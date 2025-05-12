/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:44:18 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/12 11:44:24 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	*get_args(char **av, int ac)
{
	int	*args;
	int	i;

	args = (int *)malloc(sizeof(int) * 5);
	i = 0;
	if (ac == 5)
		args[4] = -1;
	while (i <= ac)
	{
		args[i - 1] = phatoi(av[i]);
		if (args[i - 1] == -1)
		{
			free(args);
			return(NULL);
		}
	}
	return(args);
}

int	main(int ac, char **av)
{
	int	*args;

	args = NULL;
	if (ac >= 5 && ac <= 6)
	{
		args = get_args(av, ac);
		if (args)
			start(args);
		else
		{
			error();
			return (-1);
		}
	}
	else
	{
		error();
		return (-1);
	}
	free(args);
	return (0);
}
