/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:59:08 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/22 12:02:14 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	if (ac == 5)
	{
		if (!til_death_phil(av))
			printf("invalid, input\n");
	}
	else if (ac == 6)
	{
		if (!repeat_phil(av))
			printf("invalid, input\n");
	}
	else
		printf("invalid, input\n");
	return (0);
}
