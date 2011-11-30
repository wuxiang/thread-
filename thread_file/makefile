OBJS = -o main
CC = g++
DEBUG = -g
FLAG = -Wall $(DEBUG)	
LIB = -lboost_thread -lpthread


cond:condition.cpp
	$(CC) $(FLAG) $(LIB) condition.cpp $(OBJS)
mutex:mutex.cpp
	$(CC) $(FLAG) $(LIB) mutex.cpp $(OBJS)
clean:
	rm -rf *.o main



