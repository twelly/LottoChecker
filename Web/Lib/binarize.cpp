#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include "tsimageproc.h"

/*                                                                                                           */
/* tsBinarizeImage()                                                                                         */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage *tsBinarizeImage(TSImage *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */

/* Inverse Image Binarization with noise filter */
TSImage *tsBinarizeImage(TSImage *image, int threshold, int dilate)
{
    IplImage* imgGray;
    IplImage* imgThresh;
    IplImage* imgSmooth;
    IplImage* imgResult = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
    cvZero(imgResult);
    /* convert to grayscale */
    cvCvtColor(image,imgResult,CV_RGB2GRAY);
    imgGray = cvCloneImage(imgResult);
    cvZero(imgResult);
    /* apply gaussian blur */
    cvSmooth(imgGray, imgResult, CV_GAUSSIAN, 1, 1);
    imgSmooth = cvCloneImage(imgResult);
    cvZero(imgResult);
    /* apply inverse binarization */
    cvThreshold(imgSmooth, imgResult, threshold, 255, CV_THRESH_BINARY_INV);
    imgThresh = cvCloneImage(imgResult);
    cvZero(imgResult);
    cvDilate(imgThresh,imgResult,NULL,1);
    return (imgResult);
}
