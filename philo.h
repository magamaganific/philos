#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>


#ifndef LOCKED
# define LOCKED 2
#endif
#ifndef ALIVE
# define ALIVE 1
#endif
#ifndef DEAD
# define DEAD 0
#endif

typedef struct s_philosopher
{
	pthread_t		thread;
	pthread_mutex_t	reap;
	int				id;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	long			*r_start;
	long			last_meal;
	long			eat;
	long			sleep;
	int				repeat;
	int				state;
}	p_hilosopher;

typedef struct s_table
{
	p_hilosopher	subject[250];
	pthread_mutex_t	fork[250];
	pthread_mutex_t	death;
	int				subjects;
}	p_urgatory;

typedef struct s_reaper
{
	p_urgatory		table;
	int				rep;
	pthread_t		leash;
	pthread_mutex_t	*end;
	long			r_start;
	long			hourglass;
}	grim_r;