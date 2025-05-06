#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/time.h>

#define ALIVE 0
#define DEAD 1

typedef struct s_philosopher
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	unsigned long			hourglass;
	unsigned long			*start;
	unsigned long			rest;
	unsigned long			eat;
	unsigned long			last_meal;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	int				state;
}	t_philosopher;


typedef struct s_reaper
{
	unsigned long			start;
	t_philosopher	subject[250];
	int				seats;
	pthread_mutex_t	fork[250];
}	t_reaper;