# Variables
NAME        = ircserv
SRCS        = ./srcs/main.cpp
SRCS		+= ./srcs/Server.cpp
SRCS		+= ./srcs/CommandHandler.cpp
SRCS		+= ./srcs/Client.cpp
SRCS		+= ./srcs/Channel.cpp
SRCS		+= ./srcs/ServerChannel.cpp
SRCS		+= ./srcs/replies.cpp
SRCS		+= ./srcs/repliesChannel.cpp
SRCS		+= ./srcs/commands/Pass.cpp
SRCS		+= ./srcs/commands/Nick.cpp
SRCS		+= ./srcs/commands/User.cpp
SRCS		+= ./srcs/commands/Join.cpp
SRCS		+= ./srcs/commands/Kick.cpp
SRCS		+= ./srcs/commands/Mode.cpp
SRCS		+= ./srcs/commands/Topic.cpp
SRCS		+= ./srcs/commands/Privmsg.cpp
SRCS		+= ./srcs/commands/Quit.cpp
SRCS		+= ./srcs/commands/Invite.cpp
OBJS        = $(SRCS:.cpp=.o)
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I ./includes/ -g3

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re