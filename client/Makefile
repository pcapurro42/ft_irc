# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = client

SRCS = Client.cpp \

CXX = c++

CXXFLAGS = -std=c++98

OBJS = $(SRCS:.cpp=.o)

## === RULES === ##

all: client

client: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

re: fclean all

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
