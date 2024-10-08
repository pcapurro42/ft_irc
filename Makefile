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
	server/SearchTools.cpp \
	server/Execution.cpp \
	server/Initialization.cpp \
	server/Routine.cpp \
	server/Management.cpp \
	server/cmds/mode/ModeCMD.cpp \
	server/cmds/mode/ModeCMDVerify.cpp \
	server/cmds/mode/ModeCMDI.cpp \
	server/cmds/mode/ModeCMDK.cpp \
	server/cmds/mode/ModeCMDL.cpp \
	server/cmds/mode/ModeCMDO.cpp \
	server/cmds/mode/ModeCMDT.cpp \
	server/cmds/PingCMD.cpp \
	server/cmds/PongCMD.cpp \
	server/cmds/InviteCMD.cpp \
	server/cmds/JoinCMD.cpp \
	server/cmds/KickCMD.cpp \
	server/cmds/WhoCMD.cpp \
	server/cmds/NickCMD.cpp \
	server/cmds/PartCMD.cpp \
	server/cmds/PassCMD.cpp \
	server/cmds/PrivmsgCMD.cpp \
	server/cmds/QuitCMD.cpp \
	server/cmds/TopicCMD.cpp \
	server/cmds/UserCMD.cpp \
	server/cmds/BotCMD.cpp \

CXX = c++ -D MAX_CLIENTS=21 -D MAX_CANALS=42 -D BOT=0

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

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
