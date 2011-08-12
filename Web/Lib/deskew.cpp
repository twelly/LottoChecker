#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include "imgprocessing.h"

/*                                                                                                           */
/* tsDeskewImage()                                                                                           */
/*                                                                                                           */
/* Deskew image                                                                                              */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsDeskewImage(TSImage_t *image)                                                                      */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer to deskew                                                                 */
/*                                                                                                           */
/* Description:                                                                                              */
/* Deskew image using canny edge detection to calculate the skew angle.                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer with the deskewed image.                                                                     */
/*                                                                                                           */
/*                                                                                                           */
TSImage_t *tsDeskewImage(TSImage_t *image)
{
    IplImage *img_gray = cvCreateImage(cvGetSize(image), image->depth, 1);
    IplImage *img_output = cvCloneImage(image);

    //Convert into grayscale
    cvConvertImage(image, img_gray);

    //Creation of img_canny: same size and depth as img_gray, but only one channel
    IplImage *img_canny = cvCreateImage(cvGetSize(img_gray), img_gray->depth, 1);

    cvCanny(img_gray, img_canny,500,300); //******CANNY - EDGES DETECTION

    //Creation of img_ext in which we keep only the external edges
    IplImage *img_ext = cvCreateImage(cvGetSize(img_gray), img_gray->depth, 1);

    CvScalar scalar; //scalar -  place where we stock the pixel

    //Initialize the image in black (TO IMPROVE!!!!!!!)
    for (int y=0; y<img_ext->height; y++)
    {
        for (int x=0; x<img_ext->width; x++)
        {
            //Get the pixel (x,y) from img_canny
            //scalar=cvGet2D(img_canny, y, x);
            scalar.val[0]=0;//pixels - all black
            cvSet2D(img_ext, y, x, scalar);
        }
    }

    //Only the first and the last pixels lit (true) in each line
    bool found_first, found_last;
    for (int y=0; y<img_canny->height; y++)
    {
        found_first=false;
        found_last=false;
        for (int x=0; x<img_canny->width; x++)
        {
            //Get the (x,y) pixel from img_canny
            scalar=cvGet2D(img_canny, y, x);
            //Search the first not black pixel (>0)
            if ((scalar.val[0]>0) && (!found_first)) //if the pixel is not black and if the first not black pixel was not found yet
            {
                cvSet2D(img_ext, y, x, scalar);
                found_first=true;
            }
            scalar=cvGet2D(img_canny, y,img_canny->width-x-1);
            // Search the last non-black pixel (>0)
            if ((scalar.val[0]>0)&& (!found_last)) //if the pixel is not black and if the last not black pixel was not found yet
            {
                cvSet2D(img_ext, y, img_canny->width-x-1, scalar);
                found_last=true;
            }
            if ((found_first) && (found_last)) break;
        }
    }

    //Search lines using Hough transformation
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    lines = cvHoughLines2(img_ext, storage, CV_HOUGH_PROBABILISTIC, 0.1, CV_PI/180/20, 20, 20, 10 );
    printf("Number of found lines: %d\n", lines->total);
    //cout<<"Number of found lines: "<<lines->total<<endl;

    double mean_theta=0;
    int lines_detected=0;
    for ( int i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( img_output, line[0], line[1], CV_RGB(255,0,0), 13, 8 );
        double theta = atan ((line[1].x-line[0].x) / (double)(line[1].y-line[0].y))*180/CV_PI;
        if (theta != 0)
        {
           lines_detected++;
           mean_theta+=theta;
        }
        printf("Line number %d theta = %f\n", i, theta);
        //cout<<"    Line number "<<i<<"   theta "<<theta<<endl;
    }
    mean_theta/=lines_detected;
    //mean_theta/=lines->total;
    printf("Mean orientation: %f\n", mean_theta);
    //cout<<"Mean orientation: "<<mean_theta<<endl;

    IplImage *img_turned = cvCreateImage(cvGetSize(image), image->depth, 3);
    float m[6];
    double factor = 1;//(cos(mean_theta*CV_PI/180.) + 1.1)*3;
    CvMat M = cvMat( 2, 3, CV_32F, m );
    int w = image->width;
    int h = image->height;
    m[0] = (float)(factor*cos(mean_theta*CV_PI/180.));
    m[1] = (float)(factor*sin(mean_theta*CV_PI/180.));
    m[2] = w*0.5f;
    m[3] = -m[1];
    m[4] = m[0];
    m[5] = h*0.5f;
    cvGetQuadrangleSubPix( image, img_turned, &M);//, 1, cvScalarAll(0));
    cvReleaseImage(&img_gray);
    cvReleaseImage(&img_canny);
    cvReleaseImage(&img_ext);
    return (img_turned);
}

