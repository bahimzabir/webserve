NAME		=	webserve

FLAGS		=	-Wall -Werror -Wextra

CPPFLAGS	=	-std=c++98

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
				./main.cpp								\
				./server/server_init.cpp						\
				./server/servers.cpp							\
				./server/socket.cpp

OBJS		=	${SRCS:.cpp=.o}

all			:	${NAME}

${NAME}		:	${SRCS}
				c++ ${FLAGS} ${CPPFLAGS} ${SRCS} -o ${NAME}

clean		:
				rm -rf ${OBJS}

fclean		:	clean
				rm -rf ${NAME}

re			:	fclean	all