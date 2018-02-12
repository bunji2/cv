#ifndef CAM_H
#define CAM_H

#ifdef __cplusplus
extern "C" {
#endif

const char *get_opencv_version();

#define CAM_IDX 0
#define WIDTH 1
#define HEIGHT 2
#define WAIT_MS 3
#define IMG_FILE 4
#define DISP_TIME 5
/*
#define NEW_PARAM X
*/

typedef struct _videocap_param {
    int cam_idx;
    int w;
    int h;
    int wait_ms;
    const char *img_file;

    int disp_time;
/*
    int new_param;
*/

/*
    int face_detect;
    int filter;
    char* filename;
    CvVideoWriter *vw;
*/
} videocap_param;

videocap_param* new_videocap_param();
void release_videocap_param(videocap_param**);
void set_videocap_param_int(videocap_param*, int, int);
void set_videocap_param_str(videocap_param*, int, const char*);

int get_videocap_param_int(videocap_param*, int);
const char* get_videocap_param_str(videocap_param*, int);

int videocap(videocap_param *);

#ifdef __cplusplus
}
#endif

#endif /* CAM_H */