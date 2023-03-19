SRC= ./server/http/*/*.cpp ./server/http/*.cpp ./server/*.cpp ./parcing/*.cpp
SRC2= ./server/http/config_matcher.cpp ./parcing/parcer.cpp ./server/main.cpp ./server/server_init.cpp
CPPFLAGS = -std=c++98 -fsanitize=address -g
NAME = webserve


match:$(SRC2)
	g++ $(SRC2) $(CPPFLAGS) -o match
webserve:$(SRC)
	g++ $(SRC) $(CPPFLAGS) -o $(NAME)
clean:
	rm -rf $(NAME)
