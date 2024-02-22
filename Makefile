# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = ircserv

SRCS = main.cpp \
	Print.cpp \
	server/RandomTools.cpp \
	server/Server.cpp \
	server/InputRectification.cpp \
	server/AnalyzeTools.cpp \
	server/Execution.cpp \
	server/cmds/PingCMD.cpp \
	server/cmds/PongCMD.cpp \
	server/cmds/InviteCMD.cpp \
	server/cmds/JoinCMD.cpp \
	server/cmds/WhoCMD.cpp \
	server/cmds/KickCMD.cpp \
	server/cmds/ModeCMD.cpp \
	server/cmds/NickCMD.cpp \
	server/cmds/PartCMD.cpp \
	server/cmds/PassCMD.cpp \
	server/cmds/PrivmsgCMD.cpp \
	server/cmds/QuitCMD.cpp \
	server/cmds/TopicCMD.cpp \
	server/cmds/UserCMD.cpp \
	server/Initialization.cpp \
	server/Routine.cpp \
	server/Management.cpp \

CXX = c++ -D MAX_CLIENTS=21 -D MAX_CANALS=42

CXXFLAGS = -std=c++98

OBJS = $(SRCS:.cpp=.o)

## === RULES === ##

all: ircserv

ircserv: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

re: fclean all

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
