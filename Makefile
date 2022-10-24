# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lschrafs <lschrafs@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/24 16:50:36 by lschrafs          #+#    #+#              #
#    Updated: 2022/10/24 16:54:05 by lschrafs         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				= 	container_tester
COMPILER 		= 	c++
CFLAGS 			= 	-Wall -Wextra -Werror -std=c++98

SRCS_PATH 	= 	./src/

SRCS_NAMES	= 	main.cpp \

OBJS_PATH		= 	./obj/
OBJS_NAMES	= 	$(SRCS_NAMES:%.cpp=%.o)
OBJS				= 	$(addprefix $(OBJS_PATH), $(OBJS_NAMES))


all: $(NAME)

$(NAME): $(OBJS) $(MYLIBS)
	$(COMPILER) $(CFLAGS) $(OBJS) -o $@

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	mkdir -p $(OBJS_PATH)
	$(COMPILER) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all