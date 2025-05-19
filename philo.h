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

typedef struct s_testkill
{
	pthread_t		thread;
	pthread_mutex_t	kill;
	pthread_mutex_t *start;
	int				*flag;
	int				*state;
}	t_kill;

typedef struct s_scythe
{
	t_kill			test[10];
	pthread_mutex_t kill[10];
	pthread_mutex_t	start;
	int				state;
	int				flag;
}	t_scythe;
