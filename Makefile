CXX = g++
FLAGS = -O2 -g -Wall -Wextra -pedantic -std=c++20 -I include

default:
	$(CXX) $(FLAGS) -c src/Example.cpp
