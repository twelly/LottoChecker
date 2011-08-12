#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "imgprocessing.h"

/* Binarize image */
int main()
{
   TSImage_t *image = tsOpenImage("images/original.jpg");
   TSImage_t *bin_image = tsBinarizeImage(image, 60, 1);
   cvNamedWindow("output");
   cvShowImage("output", bin_image);
   cvWaitKey(0);
   tsReleaseImage(image);
   tsReleaseImage(bin_image);
   return 0;
}

