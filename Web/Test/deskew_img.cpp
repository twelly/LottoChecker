#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "tsimageproc.h"

/* Binarize image */
int main()
{
   TSImage *image = tsOpenImage("images/skewed10.jpg");
   TSImage *dsk_image = tsDeskewImage(image);
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

