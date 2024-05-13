NAME      = ircserv
CPP       = c++
RMIF      = rm -rf
CPPFLAGS  = -Wall -Werror -Wextra -I./includes -std=c++98

SRCS_DIR  = ./srcs
OBJ_DIR   = ./objs
HEAD_DIR  = ./includes

RED       = \033[1;31m
GREEN     = \033[1;32m
YELLOW    = \033[1;33m
DEFAULT   = \033[0m

# List of source files
SRCS      = $(wildcard $(SRCS_DIR)/*.cpp)
HEADERS   = $(wildcard $(HEAD_DIR)/*.hpp)
OBJS      = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

# Target to build the executable
$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN) ✔ $(NAME) created!$(DEFAULT)"

# Phony target to build all
all: $(NAME)

# Phony target to clean object files
clean:
	@$(RMIF) $(OBJ_DIR)
	@echo "$(YELLOW) ✔ object files deleted!$(DEFAULT)"

# Phony target to clean all
fclean: clean
	@$(RMIF) $(NAME)
	@echo "$(RED) ✔ executable deleted!$(DEFAULT)"

# Phony target to rebuild all
re: fclean all

# Declare phony targets
.PHONY: all clean fclean re