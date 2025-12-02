# Variables
NAME        = ircserv
SRCS        = main.cpp Server.cpp CommandHandler.cpp Client.cpp Pass.cpp Nick.cpp replies.cpp User.cpp Channel.cpp ServerChannel.cpp Join.cpp Kick.cpp Mode.cpp Topic.cpp repliesChannel.cpp Privmsg.cpp Quit.cpp Invite.cpp
OBJS        = $(SRCS:.cpp=.o)
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I. -g3	

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