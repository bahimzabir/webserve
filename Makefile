SRC= ./server/http/*/*.cpp ./server/http/*.cpp ./server/*.cpp ./parcing/*.cpp
CPPFLAGS = -std=c++98 -fsanitize=address -g
NAME = webserve

webserve:$(SRC)
	g++ $(SRC) $(CPPFLAGS) -o $(NAME)
clean:
	rm -rf $(NAME)