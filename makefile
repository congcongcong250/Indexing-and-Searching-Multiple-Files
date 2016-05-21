CPPFLAGS=-std=c++11 -Werror -Wall -O2

all: a3search

a3search: a3search.cpp
	g++ $(CPPFLAGS) -o a3search a3search.cpp
