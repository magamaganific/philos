/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:00:09 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/20 11:00:15 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	final_meal(t_phil *phil)
{
	pthread_mutex_lock(&phil->state.mutex);
	phil->state.last_meal = 0;
	pthread_mutex_unlock(&phil->state.mutex);
}

int	phil_is_alive(t_phil *phil)
{
	int	res;

	res = 0;
	pthread_mutex_lock(phil->mutex);
	if (*phil->state.state == ALIVE)
		res = 1;
	pthread_mutex_unlock(phil->mutex);
	return (res);
}

void	left_lunch_way(t_phil *phil)
{
	while (phil_is_alive(phil) && --phil->reps != 0)
	{
		pthread_mutex_lock(phil->fork1);
		if (phil_is_alive(phil))
			print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork2);
		pthread_mutex_lock(&phil->state.mutex);
		phil->state.last_meal = get_curr_time();
		pthread_mutex_unlock(&phil->state.mutex);
		if (phil_is_alive(phil))
			print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal) < phil->eat
			&& phil_is_alive(phil))
			usleep(1);
		pthread_mutex_unlock(phil->fork2);
		pthread_mutex_unlock(phil->fork1);
		if (phil_is_alive(phil))
			print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal)
			< (phil->eat + phil->sleep) && phil_is_alive(phil))
			usleep(1);
		if (phil_is_alive(phil))
			print_time(THINKING, *phil->start_time, phil->id);
		usleep(1);
	}
}

void	right_lunch_way(t_phil *phil)
{
	while (phil_is_alive(phil) && --phil->reps != 0)
	{
		pthread_mutex_lock(phil->fork2);
		if (phil_is_alive(phil))
			print_time(FORK, *phil->start_time, phil->id);
		pthread_mutex_lock(phil->fork1);
		pthread_mutex_lock(&phil->state.mutex);
		phil->state.last_meal = get_curr_time();
		pthread_mutex_unlock(&phil->state.mutex);
		if (phil_is_alive(phil))
			print_time(EATING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal) < phil->eat
			&& phil_is_alive(phil))
			usleep(1);
		pthread_mutex_unlock(phil->fork1);
		pthread_mutex_unlock(phil->fork2);
		if (phil_is_alive(phil))
			print_time(SLEEPING, *phil->start_time, phil->id);
		while ((get_curr_time() - phil->state.last_meal)
			< (phil->eat + phil->sleep) && phil_is_alive(phil))
			usleep(1);
		if (phil_is_alive(phil))
			print_time(THINKING, *phil->start_time, phil->id);
		usleep(1);
	}
}

void	*routine(void *args)
{
	t_phil	*phil;
	long	flag;

	phil = (t_phil *)args;
	while (1)
	{
		pthread_mutex_lock(phil->mutex);
		flag = *phil->start_time;
		pthread_mutex_unlock(phil->mutex);
		if (flag)
			break ;
		usleep(1);
	}
	pthread_mutex_lock(&phil->state.mutex);
	phil->state.last_meal = get_curr_time();
	pthread_mutex_unlock(&phil->state.mutex);
	if (phil->id % 2 == 0)
		left_lunch_way(phil);
	else
		right_lunch_way(phil);
	pthread_mutex_lock(phil->mutex);
	*phil->state.rep_check = *phil->state.rep_check - 1;
	pthread_mutex_unlock(phil->mutex);
	final_meal(phil);
	return (0);
}
