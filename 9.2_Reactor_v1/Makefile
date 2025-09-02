SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
CC:=g++
EXE:=server.exe
FLAGS:=-g

$(EXE):$(OBJS)
	$(CC) -o $@ $^ $(FLAGS)

%.o:%.cpp
	$(CC) -c $^ -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS) $(EXE)
echo:
	@echo $(SRCS)
	@echo $(OBJS)
