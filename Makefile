CC=g++

all: bl

bl: bl.cpp
	$(CC) -o $@ $^ -Wall

