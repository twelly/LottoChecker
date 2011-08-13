#ifndef _TSIMAGEPROC_H_
#define _TSIMAGEPROC_H_

#include <tscommon.h>
 
TSImage *tsOpenImage(const char *filename);
TSImage *tsCloneImage(TSImage *image);
void tsReleaseImage(TSImage *image);
TSImage *tsBinarizeImage(TSImage *image, int threshold, int dilate);
TSImage *tsDeskewImage(TSImage *image);

#endif /* _TSIMAGEPROC_H_ */
