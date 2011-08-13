CFLAGS = -Wall -g -O0 
LDFLAGS = -L../Lib
INCLUDE = -I/usr/include/opencv -I../Include -I.
OPENCV = -lcxcore -lcv -lhighgui -lcvaux
LIBS = $(OPENCV) -limgprocessing -ltemplatematch
BINDIR = ../Bin
OBJECTS= main.o

all: lottochecker

lottochecker: $(OBJECTS)
	g++ -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)
	mv $@ $(BINDIR)

main.o:main.cpp
	g++ -c $(CFLAGS) $(INCLUDE) main.cpp

clean:
	rm -f *.o
