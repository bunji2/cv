#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h>
#include "cam.h"

const char *get_opencv_version() {
    return CV_VERSION;
}

videocap_param* new_videocap_param() {
    return (videocap_param*)cvAlloc(sizeof(videocap_param));
}

void release_videocap_param(videocap_param **p) {
    cvFree(p);
}

void set_videocap_param_int(videocap_param* p, int name, int value) {
    switch (name) {
        case CAM_IDX:
            p->cam_idx = value;
            break;
        case WIDTH:
            p->w = value;
            break;
        case HEIGHT:
            p->h = value;
            break;
        case WAIT_MS:
            p->wait_ms = value;
            break;
        case DISP_TIME:
            p->disp_time = value;
            break;

/*
        case NEW_PARAM:
            p->new_param = value;
            break;
*/

    }
}

void set_videocap_param_str(videocap_param* p, int name, const char* value) {
    switch (name) {
        case IMG_FILE:
            p->img_file = value;
    }
}

int get_videocap_param_int(videocap_param* p, int name) {
    switch (name) {
        case CAM_IDX:
            return p->cam_idx;
        case WIDTH:
            return p->w ;
        case HEIGHT:
            return p->h;
        case WAIT_MS:
            return p->wait_ms;
        case DISP_TIME:
            return p->disp_time;
    }
    return 0;
}

const char* get_videocap_param_str(videocap_param* p, int name) {
    switch (name) {
        case IMG_FILE:
            return p->img_file;
    }
    return NULL;
}

void put_time_str(cv::Mat &frame, int w, int h) {
    char buff[24] = {};
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    sprintf(buff, "%04d:%02d:%02d %02d:%02d:%02d", pnow->tm_year + 1900, pnow->tm_mon + 1, pnow->tm_mday,
    pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
//    std::cout << buff << std::endl;
	int baseLine;
	cv::Size textSize = cv::getTextSize(buff, cv::FONT_HERSHEY_COMPLEX, 1.0, 2, &baseLine);
	cv::Point pt1(w - textSize.width, h - textSize.height - baseLine);
	cv::Point pt2(w, h);
	cv::rectangle(frame, pt1, pt2, cv::Scalar(0,0,0), CV_FILLED);
	cv::putText(frame, buff, cv::Point(w - textSize.width, h - baseLine), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255,255,255), 2, CV_AA);
//    cv::putText(frame, buff, cv::Point(50,50), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(255,255,255), 2, CV_AA);
//    cv::String str(buff);
//    cv::addText(frame, str, cv::Point(50,50), cv::FONT_HERSHEY_COMPLEX, 1.2, cv::Scalar(0,0,200), 2, CV_AA);
//    cv::addText(frame, "hoge", cv::Point(50,50), cv::FONT_HERSHEY_COMPLEX, 1.2, cv::Scalar(0,0,200));
}

int
videocap(videocap_param *p)
{
    cv::VideoCapture cap(p->cam_idx);
    // 様々な設定...
    cap.set(CV_CAP_PROP_FRAME_WIDTH, p->w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, p->h);
    p->w = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
    p->h = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    // カメラがオープンできたかの確認
    if(!cap.isOpened()) return -1;

//  cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Capture", CV_WINDOW_NORMAL);
    cv::Mat frame;
    while(cv::waitKey(p->wait_ms)<0) {
        cap >> frame;  // キャプチャ
        // 様々な処理
        // ...
        if (p->disp_time) {
            put_time_str(frame, p->w, p->h);
        }
        cv::imshow("Capture", frame); // 表示
    }
    cv::imwrite(p->img_file, frame);
    return 0;
}

