NAME = philosophers.a
LIBC = ar rc
RM = rm -rf
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = 

 OBJS = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(LIBC) $(NAME) $(OBJS)
clean :
	$(RM) $(OBJS)
fclean : clean
		$(RM) $(NAME)
re : fclean all