SRC= ./http/*.cpp ./parcing/*.cpp ./server/*.cpp
CPPFLAGS = -std=c++98
NAME = webserve

webserve:$(SRC)
	g++ $(SRC) $(CPPFLAGS) -o $(NAME)
clean:
	rm -rf $(NAME)