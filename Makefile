#Variables
COMPILE = g++
FLAGS = -g -W -Wall -Werror -ansi -pedantic

#Targets

all: main.cpp $(OBJS)
	$(COMPILE) $(FLAGS) -o a.out main.cpp

clean:
	rm -rf *~ *.o a.out
