NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread #-fsanitize=thread -g3
RM      = rm -rf

SRC     = philo.c time.c parsing.c helpers.c helpers+.c
OBJS    = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

