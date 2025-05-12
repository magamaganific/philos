#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#ifndef UNLOCKED
# define UNLOCKED 0
#endif
#ifndef LOCKED
# define LOCKED 1
#endif


void	print_time(char *label, int id);
void	pherror(void);
int		ft_is_allnum(char *av);
int		phatoi(char *av);