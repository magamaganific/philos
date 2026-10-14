/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:17:21 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/13 15:17:24 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#ifndef UNLOCKED
# define UNLOCKED 0
#endif
#ifndef LOCKED
# define LOCKED 1
#endif
#ifndef ALIVE
# define ALIVE 0
#endif
#ifndef DEAD	
# define DEAD 1
#endif


typedef struct s_philosopher
{
	long			*start;
	int				id;
	int				state;
	pthread_t		thread;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	dead;
	long			hourglass;
	long			rest;
	long			eat;
	long			last_meal;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
}	t_philosopher;

typedef struct s_reaper
{
	pthread_t		thread;
	long			start;
	long			hourglass;
	t_philosopher	*subject;
	pthread_mutex_t	*fork;
	int				*state;
	int				seats;
	pthread_mutex_t	mutex;
}	t_reaper;


void	print_time(char *label, int id);
void	pherror(void);
int		ft_is_allnum(char *av);
int		phatoi(char *av);
