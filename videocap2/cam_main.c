#include <stdio.h>
#include "cam.h"

#ifdef USE_MAIN
int
main(int argc, char *argv[])
{
    videocap_param *p = new_videocap_param();
    set_videocap_param_int(p, CAM_IDX, 0);
    set_videocap_param_int(p, WIDTH, 1280);
    set_videocap_param_int(p, HEIGHT, 768);
    set_videocap_param_int(p, WAIT_MS, 100);
    set_videocap_param_str(p, IMG_FILE, "hoge.png");
    set_videocap_param_int(p, DISP_TIME, 1);

    printf("Using OpenCV %s\n", get_opencv_version());

    videocap(p);

    release_videocap_param(&p);
}
#endif

