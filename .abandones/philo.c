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

void	hour_glass(time);

void	*start_routine(void *arg)
{
	t_philo	*phil = (t_philo *)arg;
	int		ready;
	int		i;

	ready = 0;
	i = -1;
	while (1)
	{
		pthread_mutex_lock(phil->bang);
		ready = phil->start_flag;
		pthread_mutex_unlock(phil->bang);
		if (ready)
			break ;
		usleep(100);
	}
	print_time("start", phil->id);
	while (i <= phil->rep)
	{
		pthread_mutex_lock(&phil->f1->f);
		print_time("picked up a fork", phil->id);
		pthread_mutex_lock(&phil->f2->f);
		print_time("is eating", phil->id);
		hour_glass(phil->eat_cron);
		pthread_mutex_unlock(&phil->f1->f);
		pthread_mutex_unlock(&phil->f2->f);
		print_time("is sleeping", phil->id);
	}
}

void	setup_philosophers(t_data *in, int *ars)
{
	int	i;

	i = -1;
	while (i < ars[0])
	{
		in->phil[i].id = in + 1;
		pthread_mutex_init(&in->f[i].f, NULL);
		in->phil[i].f1 = &in->f[i];
		in->phil[i].f2 = &in->f[i + 1];
		if (i == ars[0] - 1)
			in->phil[i].f2 = &in->f[0];
		in->phil[i].bang = &in->bang;
		in->phil[i].cron = ars[1];
		in->phil[i].eat_cron = ars[2];
		in->phil[i].sleep_cron = ars[3];
		in->phil[i].rep = ars[4];
		pthread_create(&in->thread, NULL, start_routine, &in->phil[i]);
	}
	usleep(1000);

	pthread_mutex_lock(&in->bang);
	in->start_flag = 1;
	pthread_mutex_unlock(&in->bang);
}

int	main(int ac, char **av)
{
	t_data	*init;
	int		*args;

	args = NULL;
	print_time("Process init", 0);
	if (ac)
	{
		args = (int *)malloc(sizeof(int) * 5);
		args[0] = 2; args[1] = 700; args[2] = 200; args[3] = 200; args[4] = 5;
	}
	setup_philosophers(init, args);
}
