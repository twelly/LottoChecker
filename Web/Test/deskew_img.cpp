#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "imgprocessing.h"

/* Binarize image */
int main()
{
   TSImage_t *image = tsOpenImage("images/skewed10.jpg");
   TSImage_t *dsk_image = tsDeskewImage(image);
   cvNamedWindow("original");
   cvShowImage("original", image);
   cvNamedWindow("output");
   cvShowImage("output", dsk_image);
   cvWaitKey(0);
   cvDestroyWindow("original");
   cvDestroyWindow("output");
   tsReleaseImage(image);
   tsReleaseImage(dsk_image);
   return 0;
}

