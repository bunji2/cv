#ifndef VIDEOCAP__H
#define VIDEOCAP__H
#include <stdio.h>

void videocap(int cam_idx, const char* title, int w, int h, int wait_ms, int use_filter);
void videocap2(int cam_idx, const char* title, int w, int h, int wait_ms, int use_filter, const char* filename);

#endif /* VIDEOCAP__H */
