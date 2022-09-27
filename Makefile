NAME = Webserv.out
SRC = Webserv/Webserv.cpp Location/Location.cpp Server/Server.cpp Multiplexing/Multiplexing.cpp Request/Request.cpp Response/Response.cpp cgi/cgi.cpp 
HEADER = 
CC = c++ 
OBJ = $(SRC:.cpp=.o)
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3
DEP = $(SRC) $(header)
SANITIZER = -g -fsanitize=address

all: $(NAME)

$(NAME): $(DEP) 
		$(CC) $(SRC) -o $(NAME) -g3 -fsanitize=address
clean:
		/bin/rm -f $(OBJ)
fclean: clean
		/bin/rm -f $(NAME)
re: fclean all
