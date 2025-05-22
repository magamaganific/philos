/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:59:49 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/20 10:59:55 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

# ifndef ALIVE
#  define ALIVE 0
# endif
# ifndef DEAD
#  define DEAD -1
# endif
# ifndef LOCKED
#  define LOCKED 0
# endif
# ifndef UNLOCKED
#  define UNLOCKED 1
# endif
# ifndef THINKING
#  define THINKING 0
# endif
# ifndef FORK
#  define FORK 1
# endif
# ifndef EATING
#  define EATING 2
# endif
# ifndef SLEEPING
#  define SLEEPING 3
# endif

typedef struct s_hourglass
{
	pthread_mutex_t	mutex;
	long			last_meal;
	int				*rep_check;
	int				*state;
}	t_hourglass;

typedef struct s_phil
{
	pthread_t		thread;
	int				id;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	pthread_mutex_t	*mutex;
	long			eat;
	long			sleep;
	long			think;
	t_hourglass		state;
	long			*start_time;
	int				reps;
	int				action;
}	t_phil;

typedef struct s_reaper
{
	struct s_phil	*phil;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex;
	int				attendance;
	int				state;
	long			start_time;
	long			hourglass;
	int				rep_check;
}	t_reaper;

int		ft_is_allnum(char *av);
int		phatoi(char *av);
long	get_curr_time(void);
void	print_time(int action, long start_time, int id);
int		*get_args(char **av, int size);
int		init_reaper(t_reaper *reaper, int size);
void	setup_forks(t_reaper *reaper, int i);
void	setup_timers(t_phil	*phil, int *args);
int		til_death_phil(char **av);
int		repeat_phil(char **av);
int		phil_is_alive(t_phil *phil);
void	left_lunch_way(t_phil *phil);
void	right_lunch_way(t_phil *phil);
void	*routine(void *args);
void	kill_philo(int id, t_reaper *reaper);
void	stop_philo(t_reaper *reaper);
void	hades_office(t_reaper *reaper);
void	end_hades(t_reaper *reaper, int *args);

#endif