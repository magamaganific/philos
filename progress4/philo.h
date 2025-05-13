#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/time.h>

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
	t_philosopher	subject[250];
	pthread_mutex_t	state[250];
	int				seats;
	pthread_mutex_t	fork[250];
	pthread_mutex_t	mutex;
}	t_reaper;