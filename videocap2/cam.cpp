#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h>
#include "cam.h"

#define WINDOW_TITLE "CAM"
#define TRACKBAR_gamma "gamma"
#define TRACKBAR_filter "filter"
#define TRACKBAR_medianblur "medianblur"


const char *
get_opencv_version() {
    return CV_VERSION;
}

videocap_param*
new_videocap_param() {
    videocap_param *r = (videocap_param*)cvAlloc(sizeof(videocap_param));
    // Initial values
    r->filter = 0;
    r->gamma = 1;
    r->medianblur = 0;
    return r;
}

void
release_videocap_param(videocap_param **p) {
    cvFree(p);
}

void
set_videocap_param_int(videocap_param* p, int name, int value) {
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
        case GAMMA:
            if (value >= 1 && value <= GAMMA_MAX) {
                p->gamma = value;
            }
            break;
        case FILTER:
            if (value >= 0 && value <= FILTER_MAX) {
                // 0: NO_FILTER, 1: FILTER1
                p->filter = value;
            }
            break;

/*
        case NEW_PARAM:
            p->new_param = value;
            break;
*/

    }
}

void
set_videocap_param_str(videocap_param* p, int name, const char* value) {
    switch (name) {
        case IMG_FILE:
            p->img_file = value;
    }
}

int
get_videocap_param_int(videocap_param* p, int name) {
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
        case FILTER:
            return p->filter;
        case GAMMA:
            return p->gamma;
    }
    return 0;
}

const char*
get_videocap_param_str(videocap_param* p, int name) {
    switch (name) {
        case IMG_FILE:
            return p->img_file;
    }
    return NULL;
}

// filter to put a current time
void
put_time_str(cv::Mat &frame, int w, int h) {
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

// 表示フレームに特殊効果を与えるフィルタ1
void
filter1(cv::Mat &frame) {
    cv::Mat gray_img;
    cv::Mat adaptive_img;
    cv::Mat bin_img;
    cv::Mat dst_img;
    cv::cvtColor(frame, gray_img, CV_BGR2GRAY);
    cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::adaptiveThreshold(gray_img, adaptive_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 8);
    cv::bitwise_and(adaptive_img, bin_img, dst_img);
    cv::cvtColor (dst_img, dst_img, CV_GRAY2BGR);
    cv::bitwise_and(dst_img, frame, frame);
}

// 表示フレームに特殊効果を与えるフィルタ2
void
filter2(cv::Mat &frame) {
    cv::Mat gray_img;
    cv::Mat adaptive_img;
    cv::Mat bin_img;
    cv::Mat dst_img;
    cv::cvtColor(frame, gray_img, CV_BGR2GRAY);
    cv::threshold(gray_img, bin_img, 90, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(gray_img, adaptive_img, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 10);
    cv::bitwise_and(adaptive_img, bin_img, dst_img);
    cv::cvtColor (dst_img, dst_img, CV_GRAY2BGR);
    cv::bitwise_and(dst_img, frame, frame);
}

// 表示フレームに特殊効果を与えるフィルタ
void
thresh_bin(cv::Mat &frame) {
    cv::threshold(frame, frame, 90, 255, cv::THRESH_BINARY);
}

// 表示フレームに特殊効果を与えるフィルタ
void
medianblur(cv::Mat &frame, int size) {
    cv::medianBlur(frame, frame, size);
    //cv::erode(frame, frame, cv::Mat(), cv::Point(-1,-1), 3);
}

// Gamma correction
void
gamma_correction(cv::Mat &frame, int gamma) {
    //int lut[256]; <--- will be failed. look-up-table must be uchar[].
    uchar lut[256];
    for (int i = 0; i < 256; i++)
    {
        lut[i] = (int)(pow((double)i/255.0, 1.0 / (double)gamma) * 255.0);
    }

    cv::Mat lutmat = cv::Mat(cv::Size(256, 1), CV_8UC1, lut);
    cv::LUT(frame, lutmat, frame);
}

int
videocap(videocap_param *p)
{
    cv::VideoCapture cap(p->cam_idx);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, p->w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, p->h);
    p->w = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
    p->h = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if(!cap.isOpened()) {
        return -1;
    }

//  cv::namedWindow(WINDOW_TITLE, CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow(WINDOW_TITLE, CV_WINDOW_NORMAL);

    cv::createTrackbar(TRACKBAR_gamma, WINDOW_TITLE, &p->gamma,
        GAMMA_MAX, NULL, NULL);
    cv::createTrackbar(TRACKBAR_filter, WINDOW_TITLE, &p->filter,
        FILTER_MAX, NULL, NULL);
    cv::createTrackbar(TRACKBAR_medianblur, WINDOW_TITLE, &p->medianblur,
        MEDIANBLUR_MAX, NULL, NULL);

    cv::Mat frame;
    //cv::Mat dst;

    while(cv::waitKey(p->wait_ms)<0) {
        cap >> frame;

        p->gamma = cv::getTrackbarPos(TRACKBAR_gamma, WINDOW_TITLE);
        p->filter = cv::getTrackbarPos(TRACKBAR_filter, WINDOW_TITLE);

        // ...

        //dst = frame;
        //std::cout << p->gamma << std::endl;

        if (p->gamma < 1) {
            cv::setTrackbarPos(TRACKBAR_gamma, WINDOW_TITLE, 1);
        }
        if (p->gamma > 1) {
            gamma_correction(frame, p->gamma);
        }

        switch (p->filter) {
            case 1:
                filter1(frame);
                break;
            case 2:
                filter2(frame);
                break;
            case 3:
                thresh_bin(frame);
                break;
            case 4:
                p->medianblur = cv::getTrackbarPos(TRACKBAR_medianblur,
                    WINDOW_TITLE);
                if (p->medianblur>0) {
                    medianblur(frame, p->medianblur*10+1);
                }
        }
        if (p->disp_time) {
            //put_time_str(dst, p->w, p->h);
            put_time_str(frame, p->w, p->h);
        }
        //cv::imshow(WINDOW_TITLE, dst);
        cv::imshow(WINDOW_TITLE, frame);
    }
    cv::imwrite(p->img_file, frame);
    return 0;
}
