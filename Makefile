NAME = ft_traceroute

SUBDIR = src/libft

HEADERS = ft_traceroute.h

CC = clang
AR = /usr/bin/ar

SRCS =	src/main.c \
		src/utils.c \
		src/validation.c 

CFLAGS = -Wall -Wextra -Werror

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C $(SUBDIR) AR=$(AR)
	sudo $(CC) $(CFLAGS) $(OBJS) $(SUBDIR)/libft.a -o $(NAME)

clean:
	rm -f $(OBJS)
	make clean -C $(SUBDIR)

fclean:	clean
	rm -f $(NAME)
	rm -rf $(SUBDIR)/libft.a

re: fclean all

.PHONY: all clean re fclean