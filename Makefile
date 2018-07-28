INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

FLAGS = -Wall
CC = g++
CFLAGS = $(FLAGS) $(INCLUDE) -g
LIBS = -lglut -lGL -lm -lGLU -lpthread

All: bin/arduinoscope

bin/arduinoscope.o: arduinoscope.cc
	$(CC) -c $(CFLAGS) -o $@ $<

bin/arduinoscope: bin/arduinoscope.o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBS)
