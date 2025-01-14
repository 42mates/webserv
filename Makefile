# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/18 13:05:21 by mbecker           #+#    #+#              #
#    Updated: 2025/01/14 13:50:53 by sokaraku         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++
CFLAG = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I inc
DEBUGGER = -g3

SRC_DIR = src
OBJ_DIR = .obj

SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))


all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(LYELLOW)Objects successfully compiled in $(YELLOW)$$PWD/obj/$(LYELLOW).$(NC)"
	@$(CC) $(CFLAG) $(SRC) $(INCLUDES) $(DEBUGGER) -o $(NAME)
	@if [ -f $(NAME) ]; then \
		echo "$(LGREEN)Created $(GREEN)$(NAME)$(LGREEN).$(NC)" ; \
	else \
		echo "$(LRED)Failed to compile $(RED)$(NAME)$(LRED).$(NC)"; fi

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(LYELLOW)Compiling $(YELLOW)$<$(NC)"
	@$(CC) $(CFLAG) $(INCLUDES) $(DEBUGGER) -c $< -o $@
	@printf "\033[1A\033[2K\r"

clean:
	@echo "$(LRED)Removing $(RED)objects$(LRED)...$(NC)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(LRED)Removing $(RED)$(NAME)$(LRED)...$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re libft

RED =\033[1;31m
LRED =\033[0;31m
GREEN =\033[1;32m
LGREEN =\033[0;32m
YELLOW =\033[1;33m
LYELLOW =\033[0;33m
GREY =\033[1;37m
NC =\033[0m