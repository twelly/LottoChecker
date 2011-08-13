#include <stdio.h>
#include <tsimageproc.h>
#include <tstemplatematch.h>

int main(int argc, char** argv) {
    char* lotto_ticket_img_file = argv[1];
    char* template_dir          = argv[2];
    int   threshold             = (argc >= 4) ? atoi(argv[3]) : 90;
    FILE* lotto_ticket_file_ptr = NULL;

    // Load ticket and lotto image files
    if ((lotto_ticket_file_ptr = fopen(lotto_ticket_img_file, "r")) == NULL) {
        printf("\nERROR: Lotto ticket <%s> does not exist\n", lotto_ticket_img_file);
        return(1);
    }
       
    fclose(lotto_ticket_file_ptr);

    // Load ticket and lotto image files
    TSImage *lotto_ticket_img = tsOpenImage(lotto_ticket_img_file);

    if (lotto_ticket_img == NULL) {
        return(1);
    }

    // Pre-process lotto ticket image
    TSImage* bin_lotto_ticket_img = tsBinarizeImage(lotto_ticket_img, threshold, 1);

    // Find template match
    int status = tsTplmInitialize(template_dir);

    if (status == 1) {
        tsTplmFindMatches(bin_lotto_ticket_img);
        tsTplmDestroy();
    }

    // Cleanup
    tsReleaseImage(lotto_ticket_img);
    tsReleaseImage(bin_lotto_ticket_img);

    return(0);
}
