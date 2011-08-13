#include <stdio.h>
#include <tsimageproc.h>
#include <tstemplatematch.h>

int main(int argc, char** argv) {
    char *lotto_ticket_img_file = argv[1];
    TSImage *bin_lotto_ticket_img;

    // Load ticket and lotto image files
    TSImage *lotto_ticket_img = tsOpenImage(lotto_ticket_img_file);

    if (lotto_ticket_img == NULL) {
        return(1);
    }

    // Pre-process lotto ticket image
    bin_lotto_ticket_img = tsBinarizeImage(lotto_ticket_img, 60, 1);
    //cvThreshold(lotto_ticket_img, lotto_ticket_img, 90, 255, CV_THRESH_BINARY_INV);
    //cvSmooth(lotto_ticket_img, lotto_ticket_img, CV_GAUSSIAN, 3, 0, 0, 0);
    //cvErode(lotto_ticket_img, lotto_ticket_img, NULL, 1);
    //cvDilate(lotto_ticket_img, lotto_ticket_img, NULL, 1);

    // Find template match
    int status = tsTplmInitialize();

    if (status == 1) {
        tsTplmFindMatches(bin_lotto_ticket_img);
        tsTplmDestroy();
    }

    // Cleanup
    tsReleaseImage(lotto_ticket_img);
    tsReleaseImage(bin_lotto_ticket_img);

    return(0);
}
