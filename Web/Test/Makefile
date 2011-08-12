CFLAGS = -Wall -g -O0 
LDFLAGS = -L../Lib
INCLUDE = -I/usr/include/opencv -I../Include -I.
OPENCV = -lcxcore -lcv -lhighgui -lcvaux
LIBS = $(OPENCV) -limgprocessing

all: binarize deskew

binarize: binarize_img.o
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)

binarize_img.o: binarize_img.cpp
	g++ -c $(CFLAGS) $(INCLUDE) binarize_img.cpp

deskew: deskew_img.o
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)

deskew_img.o: deskew_img.cpp
	g++ -c $(CFLAGS) $(INCLUDE) deskew_img.cpp

clean:
	rm -f *.o binarize deskew


