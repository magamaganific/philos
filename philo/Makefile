NAME = philo
CC = cc -Wall -Wextra -Werror -g3 -fsanitize=thread
RM = rm -f

SRCS = hades.c main.c set_routine.c setup.c utils.c
OBJS = $(SRCS:.c=.o)

PB = \033[35m
NRM = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)
	@echo "-----Expected input:\n$(PB)./philo <phil_num> <death_time> <eat_time> <sleep_time> [<set_reps>]\n$(NRM)"

%.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re