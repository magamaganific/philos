#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/time.h>

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	long			rest;
	long			eat;
	long			last_meal;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
}	t_philosopher;


typedef struct s_reaper
{
	long			hourglass;
	long			start;
	t_philosopher	subject[250];
	int				seats;
	pthread_mutex_t	fork[250];
}	t_reaper;