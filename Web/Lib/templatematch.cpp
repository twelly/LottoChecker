#include <stdio.h>
#include <templatematch.h>


// Globals
IplImage*   G_template_images[2]           = {NULL, NULL};
const char* G_template_files[2]            = {TEMPLATE_0_TO_9_FILE, TEMPLATE_A_TO_Z_FILE};
int         G_template_valid_coords[2][10] = {{0, 44, 77, 120, 163, 206, 250, 299, 344, 389},
                                              {0, 44, 77, 120, 163, 206, 250, 299, 344, 389}};

/*                                                                                                           */
/* tplmInitialize()                                                                                          */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
int tplmInitialize(void) {
    int   ret_val      = 1;
    char* template_dir = getenv("LOTTOCHECKER_TEMPLATE_DIR");

    if (template_dir == NULL) {
        printf("\nERROR: <LOTTOCHECKER_TEMPLATE_DIR> environment variable is not set\n\n");
        return 0;
    }

    // Load the template images and binarize
    int   num_templates = sizeof(G_template_files) / sizeof(char*);
    char  template_path[250];
    FILE* template_file_ptr;

    for(int template_idx = 0; template_idx < num_templates; template_idx++) {
        sprintf(template_path, "%s/%s", template_dir, G_template_files[template_idx]);

        if ((template_file_ptr = fopen(template_path, "r")) == NULL) {
            printf("\nERROR: Template file <%s> does not exist\n", template_path);
            ret_val = 0;
            break;
        }
       
        fclose(template_file_ptr);

        G_template_images[template_idx] = cvLoadImage(template_path, 0);
        cvThreshold(G_template_images[template_idx], G_template_images[template_idx], 90, 255, CV_THRESH_BINARY_INV);
    }

    return(ret_val);
}

/*                                                                                                           */
/* tplmDestroy()                                                                                             */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
void tplmDestroy(void) {
    // Destory the template images
    int num_templates = sizeof(G_template_images) / sizeof(char*);

    for(int template_idx = 0; template_idx < num_templates; template_idx++) {
        cvReleaseImage(&G_template_images[template_idx]);
    }
}

/*                                                                                                           */
/* tplmFindMatches()                                                                                         */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
void tplmFindMatches(IplImage* p_source_img) {
    IplImage*     clone_source_img = cvCloneImage(p_source_img);
    CvSeq*        contours         = NULL;
    CvMemStorage* storage          = cvCreateMemStorage(0);

    int match_info_idx = 0;
    TplmMatchInfo match_info_array[250];

    //cvNamedWindow("debug");

    // Find and process contours
    cvFindContours(clone_source_img, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP);

    for(; contours != 0; contours = contours->h_next) {
        // Validate contour
        bool valid_contour = _tplmIsValidContour(contours);

        if (valid_contour == false) {
            continue;
        }

        // Get image region for template matching
        CvRect rect         = cvBoundingRect(contours);
        CvSize roi_img_size = cvSize(rect.width, rect.height);

        cvSetImageROI(p_source_img, rect);

        IplImage *roi_img = cvCreateImage(roi_img_size, p_source_img->depth, p_source_img->nChannels);
 
        cvCopy(p_source_img, roi_img, NULL);

        TplmFindMatchInfo find_info = _tplmFindMatch(TEMPLATE_0_TO_9, roi_img);

        match_info_array[match_info_idx].match      = find_info.match;
        match_info_array[match_info_idx].x_coord    = rect.x;
        match_info_array[match_info_idx].y_coord    = rect.y;
        match_info_array[match_info_idx].width      = rect.width;
        match_info_array[match_info_idx].height     = rect.height;
        match_info_array[match_info_idx].confidence = find_info.confidence;
        match_info_idx++;

        //printf("m=%d, x=%d, y=%d, w=%d, h=%d, c=%f\n", find_info.match, rect.x, rect.y, rect.width, rect.height, find_info.confidence);

        //cvShowImage("debug", roi_img);
        //cvWaitKey(0);

        cvReleaseImage(&roi_img);

        cvResetImageROI(p_source_img);
    }

    // Sort matches
    _tplmSortMatches(match_info_array, match_info_idx);

    // Clean up
    cvReleaseMemStorage(&storage);

    cvReleaseImage(&clone_source_img);

    //cvDestroyWindow("debug");
}

// ****************************************
// These are private functions from here on
// ****************************************

/*                                                                                                           */
/* _tplmCmpMatchInfo()                                                                                       */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
int _tplmCmpMatchInfo(const void* p_a, const void* p_b) {
    TplmMatchInfo* info_a = (TplmMatchInfo*)p_a;
    TplmMatchInfo* info_b = (TplmMatchInfo*)p_b;

    int ret_val    = 0;
    int y_diff     = info_a->y_coord - info_b->y_coord;
    int abs_y_diff = abs(y_diff);

    if(abs_y_diff <= MATCH_DIFF_THRESHOLD) {
        if (info_a->x_coord == info_b->x_coord) {
            ret_val = 0;
        }
        else if (info_a->x_coord < info_b->x_coord) {
            ret_val = -1;
        }
        else {
            ret_val = 1;
        }
    }
    else {
        if(y_diff < 0) {
            ret_val = -1;
        }
        else {
            ret_val = 1;
        }
    }

    return(ret_val);
}

/*                                                                                                           */
/* _tplmSortMatches()                                                                                        */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
void _tplmSortMatches(TplmMatchInfo* p_match_info_arr, int p_num_items) {
    qsort(p_match_info_arr, p_num_items, sizeof(TplmMatchInfo), _tplmCmpMatchInfo);

    int row = 0;
    
    for(int idx = 0; idx < p_num_items; idx++) {
        int diff = abs(row - p_match_info_arr[idx].y_coord);

        if (diff > MATCH_DISP_THRESHOLD) {
            row = p_match_info_arr[idx].y_coord;
            printf("\n");
        }

        if (p_match_info_arr[idx].match == -1) {
            printf("? ");
        }
        else {
            printf("%d ", p_match_info_arr[idx].match);
        }
    }

    printf("\n");
}

/*                                                                                                           */
/* _tplmIsValidContour()                                                                                     */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
bool _tplmIsValidContour(CvSeq* p_contour) {
    bool   valid = true;
    CvRect rect  = cvBoundingRect(p_contour);
    int    area  = rect.width * rect.height;
    
    //printf("%d %d %d %d %d\n", rect.x, rect.y, rect.width, rect.height, area);

    if ((rect.width  <= CONTOUR_WIDTH_THRESHOLD)  or
        (rect.height <= CONTOUR_HEIGHT_THRESHOLD) or 
        (area        < CONTOUR_AREA_THRESHOLD)) {
        valid = false;
    }

    return(valid);
}

/*                                                                                                           */
/* _tplmFindMatch()                                                                                          */
/*                                                                                                           */
/* Binarize and dilate image                                                                                 */
/*                                                                                                           */
/* Synopsis:                                                                                                 */
/* #include "imgprocessing.h"                                                                                */
/* TSImage_t *tsBinarizeImage(TSImage_t *image, int threshold, int dilate)                                   */
/*                                                                                                           */
/* Arguments:                                                                                                */
/* TSImage_t *image - Image buffer                                                                           */
/* int threshold - Binarization threshold level (0-100)                                                      */
/* int dilate - Dilate size (1-5)                                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/* Inversely binarize and dilate image.                                                                      */
/*                                                                                                           */
/* Returns:                                                                                                  */
/* Image buffer of binarized image.                                                                          */
/*                                                                                                           */
TplmFindMatchInfo _tplmFindMatch(int p_template, IplImage* p_target_img) {
    // Get the template image
    IplImage* template_img = G_template_images[p_template];

    // Get image dimensions
    int template_img_width  = template_img->width;
    int template_img_height = template_img->height;
    int target_img_width    = p_target_img->width;
    int target_img_height   = p_target_img->height;
    int result_img_width    = template_img_width  - target_img_width  + 1;
    int result_img_height   = template_img_height - target_img_height + 1;

    // Resize the target image to match template image
    IplImage* resized_target_img;

    if (template_img_height != target_img_height) {
        // Just do a simple ratio and proportion calculation
        target_img_width  = (target_img_width * template_img_height) / target_img_height;
        target_img_height = template_img_height;
        result_img_width  = template_img_width  - target_img_width  + 1;
        result_img_height = template_img_height - target_img_height + 1;

        CvSize resized_target_img_size = cvSize(target_img_width, target_img_height);

        resized_target_img = cvCreateImage(resized_target_img_size, p_target_img->depth, p_target_img->nChannels);

        cvResize(p_target_img, resized_target_img);
    }
    else {
        resized_target_img = cvCloneImage(p_target_img);
    }

    int     template_match = -1;
    CvPoint minloc;
    CvPoint maxloc;
    double  minval;
    double  maxval;

    if ((result_img_width > 0) and (result_img_height > 0)) {
        // Find a match of the target image from the template image
        CvSize result_img_size = cvSize(result_img_width, result_img_height);
        IplImage* result_img   = cvCreateImage(result_img_size, 32, 1);
    
        cvMatchTemplate(template_img, resized_target_img, result_img, CV_TM_CCOEFF_NORMED);
    
        cvMinMaxLoc(result_img, &minval, &maxval, &minloc, &maxloc, 0);
    
        int* template_x_pos     = G_template_valid_coords[p_template];
        int  percent_confidence = (int)(maxval * 100);
    
        if (percent_confidence > MATCH_CONFIDENCE_THRESHOLD) {
            // Find the max x coordinates that matches the template
            for(int pos_idx = 0; pos_idx <= 9; pos_idx++) {
                int diff = abs(maxloc.x - template_x_pos[pos_idx]);
        
                if (diff <= TEMPLATE_X_COORD_DIFF_THRESHOLD) {
                    template_match = pos_idx;
                    break;
                }
            }
        }

        //printf("Matching Template is %d, minval=%f, maxval=%f\n", template_match, minval, maxval);
    }

    // Clean up
    cvReleaseImage(&resized_target_img);

    TplmFindMatchInfo match_info;
    match_info.match      = template_match;
    match_info.confidence = maxval;

    return(match_info);
}
