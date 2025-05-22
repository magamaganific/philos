/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hades.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:58:23 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/20 10:58:32 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	kill_philo(int id, t_reaper *reaper)
{
	int		reps;
	long	start_time;

	pthread_mutex_lock(&reaper->mutex);
	reaper->state = DEAD;
	reps = reaper->rep_check;
	start_time = reaper->start_time;
	pthread_mutex_unlock(&reaper->mutex);
	if (reps != 0)
		print_time(DEAD, start_time, id);
}

void	stop_philo(t_reaper *reaper)
{
	int	i;

	i = -1;
	if (reaper->attendance < 2)
		return ;
	i = -1;
	while (++i < reaper->attendance)
		pthread_join(reaper->phil[i].thread, NULL);
	i = -1;
	while (++i < reaper->attendance)
		pthread_mutex_destroy(&reaper->forks[i]);
}

void	hades_office(t_reaper *reaper)
{
	int		i;
	int		reps;
	long	time;

	i = -1;
	reps = 0;
	while (++i < reaper->attendance)
	{
		pthread_mutex_lock(&reaper->mutex);
		reps = reaper->rep_check;
		pthread_mutex_unlock(&reaper->mutex);
		pthread_mutex_lock(&reaper->phil[i].state.mutex);
		time = reaper->phil[i].state.last_meal;
		pthread_mutex_unlock(&reaper->phil[i].state.mutex);
		if ((time && get_curr_time() - time > reaper->hourglass) || reps == 0)
		{
			kill_philo(reaper->phil[i].id, reaper);
			stop_philo(reaper);
			break ;
		}
		if (i == reaper->attendance - 1)
			i = -1;
	}
}

void	end_hades(t_reaper *reaper, int *args)
{
	free(args);
	if (reaper->attendance < 2)
		return ;
	free(reaper->phil);
	free(reaper->forks);
}
