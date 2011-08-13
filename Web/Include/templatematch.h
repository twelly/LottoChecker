#include <cv.h>
#include <highgui.h>


// Defines
#define TEMPLATE_0_TO_9_FILE "template_0_to_9.png"
#define TEMPLATE_A_TO_Z_FILE "template_A_to_Z.png"

#define TEMPLATE_0_TO_9 0
#define TEMPLATE_A_TO_Z 1

#define TEMPLATE_X_COORD_DIFF_THRESHOLD 4
#define MATCH_CONFIDENCE_THRESHOLD      40
#define MATCH_DIFF_THRESHOLD            4
#define MATCH_DISP_THRESHOLD            40
#define CONTOUR_WIDTH_THRESHOLD         10
#define CONTOUR_HEIGHT_THRESHOLD        10
#define CONTOUR_AREA_THRESHOLD          500


// Custom Data Types
typedef struct _TplmFindMatchInfo {
    int    match;
    double confidence;
} TplmFindMatchInfo;

typedef struct _TplmMatchInfo {
    int    match;
    int    x_coord;
    int    y_coord;
    int    width;
    int    height;
    double confidence;
} TplmMatchInfo;


// Public Function Declarations
int               tplmInitialize(void);
void              tplmDestroy(void);
void              tplmFindMatches(IplImage*);

// Private Function Declarations
int               _tplmCmpMatchInfo(const void*, const void*);
bool              _tplmIsValidContour(CvSeq*);
void              _tplmSortMatches(TplmMatchInfo*, int);
TplmFindMatchInfo _tplmFindMatch(int, IplImage*);
