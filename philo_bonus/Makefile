GREEN=\033[0;32m
NC=\033[0m

NAME     = philo

CC       = cc
CFLAGS   = -Wall -Wextra -Werror

SRCS    =  parsing.c				   \
			philo.c			  		\
			utils.c					 \
			threads.c					  \
			actions.c		   			  \
			monitor.c		


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Philo compiled$(NC)"

%.o: %.c philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(GREEN)Object files cleaned$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)All cleaned$(NC)"

re: fclean all

.PHONY: clean