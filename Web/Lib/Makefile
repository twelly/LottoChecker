CFLAGS = -Wall -g -O0 -fPIC 
LDFLAGS = -shared
INCLUDE = -I/usr/include/opencv -I../Include -I.
OBJS = imaging.o binarize.o deskew.o templatematch.o

all: libtsimagelib.so

libtsimagelib.so: $(OBJS)
	g++ -o $@ $^ $(LDFLAGS)

imaging.o: imaging.cpp
	g++ -c $(CFLAGS) $(INCLUDE) imaging.cpp

binarize.o: binarize.cpp
	g++ -c $(CFLAGS) $(INCLUDE) binarize.cpp

deskew.o: deskew.cpp
	g++ -c $(CFLAGS) $(INCLUDE) deskew.cpp

templatematch.o: templatematch.cpp
	g++ -c $(CFLAGS) $(INCLUDE) templatematch.cpp

clean:
	rm -f *.o *.so


