#include <cxtypes.h>


typedef IplImage  TSImage_t;
 
TSImage_t *tsOpenImage(const char *filename);
TSImage_t *tsCloneImage(TSImage_t *image);
void tsReleaseImage(TSImage_t *image);
TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate);
TSImage_t *tsDeskewImage(TSImage_t *image);
