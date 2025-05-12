/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfornovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:02:51 by mfornovi          #+#    #+#             */
/*   Updated: 2025/05/12 17:03:02 by mfornovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

typedef struct s_forks
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philos
{
	int				id;
	pthread_mutex_t	*mutex;
	struct s_forks	*f1;
	struct s_forks	*f2;
	int				reps;
	long			eat;
	long			sleep;
	pthread_t		thread;
}	t_philos;

typedef struct s_table
{
	struct s_philos	philos[250];
	struct s_forks	forks[250];
	pthread_mutex_t	mutex;
	
}	t_table;
