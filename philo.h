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

typedef struct s_data
{
	struct s_philo	philos[250];
	pthread_mutex_t	forks[250];
	pthread_mutex_t mutex;
	long			eat_reps;
	long			eat_cron;
	long			sleep_cron;
	int				start_flag;
}	t_data;

void	print_time(char *label, int id);
void	pherror(void);
int		ft_is_allnum(char *av);
int		phatoi(char *av);