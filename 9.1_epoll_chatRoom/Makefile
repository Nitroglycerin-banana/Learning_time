SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp, %, $(SRCS))
CC:=g++
FLAGS:=-g

ALL:$(OBJS)

%:%.cc
	$(CC)  $^ -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS)
