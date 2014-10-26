DESTDIR=..
NAME=zuberwine.so
CC=gcc
CXX=g++
CP=cp -r
RM=rm -f

CSOURCES=\
		str2av.c\
		x11keysym2ucs.c
CPPSOURCES=\
		Console.cpp\
		GUI.cpp\
		Python.cpp\
		UserModuleManager.cpp\
		WineWindow.cpp\
		hooks.cpp
OBJECTS=$(CSOURCES:.c=.o) $(CPPSOURCES:.cpp=.o)

CPPFLAGS=-Iinclude -g -Wall -Wextra -fPIC
CFLAGS=
CXXFLAGS=
LDFLAGS=-shared -lpython2.7 -lCEGUIBase -lCEGUIOpenGLRenderer

export DESTDIR
.PHONY: all clean fclean re

all: $(DESTDIR)/$(NAME)

$(DESTDIR)/$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(DESTDIR)/$(NAME)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(DESTDIR)/$(NAME)

re: clean fclean all

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $(@F)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(@F)

