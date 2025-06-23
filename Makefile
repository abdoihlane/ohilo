NAME = philo
LIBC = ar rc
RM = rm -rf
CC = cc
# CFLAGS = -Wall -Wextra -Werror

SRC = philo.c time.c parsing.c 

 OBJS = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o $(NAME)
	# $(LIBC) $(NAME) $(OBJS)
clean :
	$(RM) $(OBJS)
fclean : clean
		$(RM) $(NAME)
re : fclean all