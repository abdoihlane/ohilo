# NAME = philo
# LIBC = ar rc
# RM = rm -rf
# CC = cc
# CFLAGS =  -Wall -Wextra -Werror -pthread

# SRC = philo.c time.c parsing.c 

#  OBJS = $(SRC:.c=.o)

# all : $(NAME)

# $(NAME): $(OBJS)
# 	$(CC) $(SRC) $(CFLAGS) -o $(NAME)
# 	# $(LIBC) $(NAME) $(OBJS)
# clean :
# 	$(RM) $(OBJS)
# fclean : clean
# 		$(RM) $(NAME)
# re : fclean all

NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread
RM      = rm -rf

SRC     = philo.c time.c parsing.c
OBJS    = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# Extra: run with ThreadSanitizer for data race check
tsan: clean
	$(CC) -fsanitize=thread -g $(SRC) -o $(NAME) -pthread
	@echo "Built with ThreadSanitizer! Run ./philo to test."
