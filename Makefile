INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

FLAGS = -Wall
CC = g++
CFLAGS = $(FLAGS) $(INCLUDE) -g
LIBS = -lglut -lGL -lm -lGLU -lpthread

All: pre bin/arduinoscope

clean:
	rm -fr bin

pre:
	mkdir -p bin

bin/%.o: %.cc
	$(CC) -c $(CFLAGS) -o $@ $<

bin/%.o: %.cc %.h
	$(CC) -c $(CFLAGS) -o $@ $<

objects = bin/SignalStat.o bin/Global.o bin/AdcEval.o bin/main.o

bin/arduinoscope: $(objects)
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $(objects) $(LIBS)
