#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
	int				in_use;
}	t_fork;

typedef struct s_phil
{
	pthread_t		thread;
	int				id;
	int				eaten;
	int				meals_eaten;
	int				last_meal;
	size_t			death_t;
	size_t			food_t;
	size_t			bed_t;
	size_t			start_t;
	int				philos_am;
	int				eat_times;
	int				*dead;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t *write_CÐ;
	pthread_mutex_t *death_CÐ;
	pthread_mutex_t *meal_CÐ;
}	t_phil;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_CÐ;
	pthread_mutex_t	meal_CÐ;
	pthread_mutex_t	write_CÐ;
	t_phil			*philo;
	t_fork			*fork;
}	t_program;
