NAME	= ircserver
CC		= c++
CFLAGS	= -Wall -Werror -Wextra -std=c++98
RM		= rm -rf

GREEN	= \033[;32m
RED		= \033[;31m
END		= \033[0m

SRC_DIR	= src/
OBJ_DIR	= objs/

#direccion de carpetas dentro de src/
#COMMAND es la variable para los archivos sin extension de la carpeta de parseo

COMMAND	= Nick Mode Topic Privmsg

SERVER	= server

SRC 	= 	main.cpp \
#			$(addsuffix .cpp, $(addprefix command/, $(SERVER))) \
#			$(addsuffix .cpp, $(addprefix command/, $(COMMAND))) \

OBJ 	=	$(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

all: $(NAME)
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled ✅$(END)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
#	@mkdir -p $(OBJ_DIR)/command
	@mkdir -p $(OBJ_DIR)/server
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(RED)Compiled objects have been removed$(END)"

fclean:	clean
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME) cleaned 🗑$(END)"

re: fclean all

.PHONY: all clean fclean re