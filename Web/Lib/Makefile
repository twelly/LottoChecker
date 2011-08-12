CFLAGS = -Wall -g -O0 -fPIC 
LDFLAGS = -shared
INCLUDE = -I/usr/include/opencv -I../Include -I.
OBJS = imaging.o binarize.o deskew.o

all: libimgprocessing.so

libimgprocessing.so: $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

imaging.o: imaging.cpp
	g++ -c $(CFLAGS) $(INCLUDE) imaging.cpp

binarize.o: binarize.cpp
	g++ -c $(CFLAGS) $(INCLUDE) binarize.cpp

deskew.o: deskew.cpp
	g++ -c $(CFLAGS) $(INCLUDE) deskew.cpp

clean:
	rm -f *.o *.so


