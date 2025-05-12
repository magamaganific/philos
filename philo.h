#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_data
{
	int				philo_count;
	long			reps;
	long long		obituary;
	long long		ate_time;
	long long		slept_time;
	long long		eaten_time;
	pthread_mutex_t	eaten_CÐ;
	int				state;
	pthread_mutex_t	died_CÐ;
}	the_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	fork_lock;
	struct s_data	*data;
}	t_philo;


void	pherror(void);
int		ft_is_allnum(char *av);
int		phatoi(char *av);