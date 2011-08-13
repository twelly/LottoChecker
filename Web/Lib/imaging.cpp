#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include "tsimageproc.h"

/*                                                                                                           */
/* tsOpenImage()                                                                                             */
/*                                                                                                           */
/* Open an image file                                                                                        */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage *tsOpenImage(const char *filename)                                                                */
/*                                                                                                           */
/* Description:                                                                                              */
/* Opens a given image file and stores the bitmap information into an image buffer.                          */
/* Use tsReleaseImage() to release the image buffer.                                                         */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* On success, it returns the image buffer. On failure, it returns NULL.                                     */
/*                                                                                                           */

TSImage *tsOpenImage(const char *filename)
{
    if (filename == NULL)
    {
        return NULL;
    }
    TSImage* imageFile = cvLoadImage(filename, 1);
    return imageFile;
}

/*                                                                                                           */
/* tsReleaseImage()                                                                                          */
/*                                                                                                           */
/* Open an image file                                                                                        */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage *tsOpenImage(const char *filename)                                                                */
/*                                                                                                           */
/* Description:                                                                                              */
/* Releases a given image buffer.                                                                            */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* None.                                                                                                     */
/*                                                                                                           */

void tsReleaseImage(TSImage *image)
{
    if (image == NULL)
    {
        return;
    }
    cvReleaseImage(&image);
}


/*                                                                                                           */
/* tsCloneImage()                                                                                            */
/*                                                                                                           */
/* Clone an image                                                                                            */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage *tsOpenImage(TSImage *image)                                                                      */
/*                                                                                                           */
/* Description:                                                                                              */
/* Create a copy of the image buffer.                                                                        */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Copy of the original image buffer.                                                                        */
/*                                                                                                           */

TSImage *tsCloneImage(TSImage *image)
{
    IplImage* img_cloned;
    if (image == NULL)
    {
        return NULL;
    }
    img_cloned = cvCloneImage(image);
    return (img_cloned);
}

