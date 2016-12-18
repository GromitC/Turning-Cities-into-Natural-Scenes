ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/local/lib -lglfw3 -lGLEW -framework CoreVideo -framework Cocoa  -framework OpenGL -framework IOKit 
else
LDFLAGS=-lX11 -lGL -lGLU -lglut -lGLEW -lm
endif

CC = g++
CFLAGS=-g -I/opt/local/include
CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++


SOURCES = *.cpp

main:
	$(CC) -std=c++11 $(SOURCES) $(LDFLAGS) -o main


clean: 
	-rm -r main 
