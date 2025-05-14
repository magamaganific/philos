#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

#ifndef	ALIVE
# define ALIVE 0
#endif
#ifndef	DEAD
# define DEAD -1
#endif
#ifndef	LOCKED
# define LOCKED 0
#endif
#ifndef	UNLOCKED
# define UNLOCKED 1
#endif
#ifndef THINKING
# define THINKING 0
#endif
#ifndef	FORK
# define FORK 1
#endif
#ifndef	EATING
# define EATING 2
#endif
#ifndef	SLEEPING
# define SLEEPING 3
#endif

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
	long			last_meal;
	long			*start_time;
	int				state;
	int				action;
}	t_phil;

typedef struct s_reaper
{
	struct s_phil	*phil;
	pthread_mutex_t	*forks;
	pthread_mutex_t mutex;
	int				attendance;
	long			start_time;
	long			hourglass;
}	t_reaper;