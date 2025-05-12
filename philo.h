#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				rep;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	int				*start_flag;
}	t_philo;


void	pherror(void);
int		ft_is_allnum(char *av);
int		phatoi(char *av);