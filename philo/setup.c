/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:00:30 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/20 11:00:33 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_reaper(t_reaper *reaper, int size)
{
	reaper->start_time = 0;
	reaper->state = ALIVE;
	reaper->attendance = size;
	reaper->phil = (t_phil *)malloc(sizeof(t_phil) * size);
	reaper->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
	pthread_mutex_init(&reaper->mutex, NULL);
	if (!reaper->phil || !reaper->attendance || !reaper->forks)
	{
		pthread_mutex_destroy(&reaper->mutex);
		return (0);
	}
	return (1);
}

void	setup_forks(t_reaper *reaper, int i)
{
	pthread_mutex_init(&reaper->forks[i], NULL);
	pthread_mutex_init(&reaper->phil[i].state.mutex, NULL);
	reaper->phil[i].fork1 = &reaper->forks[i];
	if (i == reaper->attendance - 1)
		reaper->phil[i].fork2 = &reaper->forks[0];
	else
		reaper->phil[i].fork2 = &reaper->forks[i + 1];
	reaper->phil[i].mutex = &reaper->mutex;
	reaper->phil[i].start_time = &reaper->start_time;
	reaper->phil[i].state.last_meal = 0;
	if (reaper->phil[i].reps != -1)
		reaper->rep_check = reaper->attendance;
	reaper->phil[i].state.rep_check = &reaper->rep_check;
}

void	setup_timers(t_phil	*phil, int *args)
{
	phil->eat = args[2];
	phil->sleep = args[3];
	phil->think = args[1] - (args[2] - args[3]);
	phil->reps = -1;
	if (args[4])
		phil->reps = args[4] + 1;
}

int	til_death_phil(char **av)
{
	t_reaper	reaper;
	int			*args;
	int			i;

	i = -1;
	args = get_args(av, 4);
	if (!args)
		return (0);
	if (!init_reaper(&reaper, args[0]))
		return (0);
	reaper.hourglass = args[1];
	while (++i < reaper.attendance)
	{
		reaper.phil[i].state.state = &reaper.state;
		reaper.phil[i].id = i + 1;
		setup_forks(&reaper, i);
		setup_timers(&reaper.phil[i], args);
		pthread_create(&reaper.phil[i].thread, NULL, routine, &reaper.phil[i]);
	}
	pthread_mutex_lock(&reaper.mutex);
	reaper.start_time = get_curr_time();
	pthread_mutex_unlock(&reaper.mutex);
	hades_office(&reaper);
	end_hades(&reaper, args);
	return (1);
}

int	repeat_phil(char **av)
{
	t_reaper	reaper;
	int			*args;
	int			i;

	i = -1;
	args = get_args(av, 5);
	if (!args)
		return (0);
	if (!init_reaper(&reaper, args[0]))
		return (0);
	reaper.hourglass = args[1];
	while (++i < reaper.attendance)
	{
		reaper.phil[i].state.state = &reaper.state;
		reaper.phil[i].id = i + 1;
		setup_timers(&reaper.phil[i], args);
		setup_forks(&reaper, i);
		pthread_create(&reaper.phil[i].thread, NULL, routine, &reaper.phil[i]);
	}
	pthread_mutex_lock(&reaper.mutex);
	reaper.start_time = get_curr_time();
	pthread_mutex_unlock(&reaper.mutex);
	hades_office(&reaper);
	end_hades(&reaper, args);
	return (1);
}
