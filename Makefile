# SRC= ./server/http/*/*.cpp ./server/http/*.cpp ./server/*.cpp ./parcing/*.cpp
# SRC2= ./server/http/config_matcher.cpp ./parcing/parcer.cpp ./server/main.cpp ./server/server_init.cpp
# CPPFLAGS = -std=c++98 -fsanitize=address -g
# NAME = webserve


# match:$(SRC2)
# 	g++ $(SRC2) $(CPPFLAGS) -o match
# webserve:$(SRC)
# 	g++ $(SRC) $(CPPFLAGS) -o $(NAME)
# clean:
# 	rm -rf $(NAME)


NAME		=	webserve

NAME2		=	match

FLAGS		=	#-Wall -Werror -Wextra

CPPFLAGS	=	-std=c++98 -fsanitize=address -g3

SRCS		=	./parcing/parcer.cpp							\
				./server/http/CGI_Handler/cgi_handler.cpp		\
				./server/http/DELETE_Handler/delete_handler.cpp	\
				./server/http/GET_Handler/get_handler.cpp		\
				./server/http/POST_Handler/post_handler.cpp		\
				./server/http/config_matcher.cpp				\
				./server/http/error_pages_map.cpp				\
				./server/http/http_request_checker.cpp			\
				./server/http/http_request.cpp					\
				./server/http/http_response.cpp					\
				./server/fd_handler.cpp							\
				./server/main.cpp								\
				./server/server_init.cpp						\
				./server/servers.cpp							\
				./server/socket.cpp

SRCS2		=	./server/http/config_matcher.cpp	\
				./parcing/parcer.cpp				\
				./server/main.cpp					\
				./server/server_init.cpp

OBJS		=	${SRCS:.cpp=.o}

# all			:	${NAME}

webserve	:	${SRCS}
				c++ ${FLAGS} ${CPPFLAGS} ${SRCS} -o ${NAME}

match		:	${SRCS2}
				c++ ${FLAGS} ${CPPFLAGS} ${SRCS2} -o ${NAME2}

clean		:
				rm -rf ${OBJS}

fclean		:	clean
				rm -rf ${NAME} ${NAME2}

re			:	fclean	all