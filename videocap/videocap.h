#ifndef VIDEOCAP__H
#define VIDEOCAP__H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

#define GAMMA_TRACKBAR_NAME "gamma"
#define FILTER_TRACKBAR_NAME "filter"

typedef struct _param {
    int cam_idx;
    char* title;
    int w;
    int h;
    int wait_ms;
    int face_detect;
    int filter;
    char* filename;
    CvVideoWriter *vw;
} param;

param* new_param(
    int cam_idx, 
    int w, 
    int h, 
    int wait_ms, 
    int face_detect,
    int filter, 
    char* filename
);

void release_param(param **p);

void videocap(param* p);

#endif /* VIDEOCAP__H */
