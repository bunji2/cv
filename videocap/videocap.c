/*
カメラからの画像キャプチャ cvCreateCameraCapture, cvQueryFrame
指定された番号のカメラから画像をキャプチャして表示する
http://opencv.jp/sample/video_io.html
*/

#include "videocap.h"
#include <ctype.h>

param* new_param(
    int cam_idx, 
    int w, 
    int h, 
    int wait_ms, 
    int filter, 
    char* filename
) {
    param *p = (param*)cvAlloc(sizeof(param));
    p->cam_idx = cam_idx;
    p->title = "Cap";
    p->w = w;
    p->h = h;
    p->wait_ms = wait_ms;
    p->filter = filter;
    p->filename = filename;
    p->vw = 0;
    return p;
}

void release_param(param **p) {
    cvFree(p);
}

// ガンマ補正
// 参考URL http://imagingsolution.blog107.fc2.com/blog-entry-166.html
IplImage* cvGamma(IplImage* src, int g) {
    IplImage* dst;
    uchar LUT[256];

    //printf("#gamma = %d\n", g);
    dst = cvCreateImage(
        cvGetSize (src), 
        src->depth, 
        src->nChannels);
	cvCopy(src, dst, NULL);

    //ガンマ補正テーブルの作成  
    for (int i = 0; i < 256; i++){  
        LUT[i] = (int)(pow((double)i / 255.0, 1.0 / (double)g) * 255.0);  
    }

    //CvMatへ変換  
    CvMat lut_mat = cvMat(1, 256, CV_8UC1, LUT);  

    //ルックアップテーブル変換  
    cvLUT(src, dst, &lut_mat);

    return dst;
}

IplImage *conv_ohtsu(IplImage *src_img) {
    //IplImage *dst_img = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
    IplImage *dst_img = cvCloneImage (src_img);
    cvSmooth (src_img, src_img, CV_GAUSSIAN, 5, 0, 0, 0);
    //cvThreshold (src_img, dst_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cvThreshold (src_img, dst_img, 90, 255, CV_THRESH_BINARY);
    return dst_img;
}

IplImage * filter (IplImage *src_img)
{
  IplImage *dst_img;
  IplImage *src_img_gray = 0;
  IplImage *tmp_img1, *tmp_img2, *tmp_img3;

  // (1)画像を読み込む
  if (src_img == 0)
    return 0;

  tmp_img1 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img2 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img3 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  src_img_gray = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  cvCvtColor (src_img, src_img_gray, CV_BGR2GRAY);
  dst_img = cvCloneImage (src_img);

  // (2)ガウシアンフィルタで平滑化を行う
  cvSmooth (src_img_gray, src_img_gray, CV_GAUSSIAN, 5, 0, 0, 0);

  // (3)二値化:cvThreshold
  cvThreshold (src_img_gray, tmp_img1, 90, 255, CV_THRESH_BINARY);

  // (4)二値化:cvAdaptiveThreshold
  cvAdaptiveThreshold (src_img_gray, tmp_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
    CV_THRESH_BINARY, 11, 10);

  // (5)二つの二値化画像の論理積
  cvAnd (tmp_img1, tmp_img2, tmp_img3, NULL);
  cvCvtColor (tmp_img3, dst_img, CV_GRAY2BGR);

  // (6)元画像と二値画像の論理積
  cvSmooth (src_img, src_img, CV_GAUSSIAN, 11, 0, 0, 0);
  cvAnd (dst_img, src_img, dst_img, NULL);

  cvReleaseImage (&src_img_gray);
  cvReleaseImage (&tmp_img1);
  cvReleaseImage (&tmp_img2);
  cvReleaseImage (&tmp_img3);
  return dst_img;
}

void cap (
    param *p
) {
    CvCapture *capture = 0;
    IplImage *frame = 0, *frame2 = 0, *frame3 = 0;
    // (1)コマンド引数によって指定された番号のカメラに対するキャプチャ構造体を作成する
    capture = cvCreateCameraCapture (p->cam_idx);

    // (2)キャプチャサイズを設定する．
    /* この設定は，利用するカメラに依存する */
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, (double)(p->w));
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, (double)(p->h));

    // (3)カメラから画像をキャプチャする
    // キー入力があるまで繰り返し
    while (cvWaitKey (p->wait_ms) == -1) {
        frame = cvQueryFrame (capture);
        frame2 = cvGamma(frame,  cvGetTrackbarPos(GAMMA_TRACKBAR_NAME, p->title));
        switch (p->filter) {
            case 1:
                frame3 = filter(frame2);
                break;
            case 2:
                frame3 = conv_ohtsu(frame2);
                break;
            default:
                frame3 = frame2;
        }
        cvShowImage (p->title, frame3);

        if (p->vw) {
            cvWriteFrame (p->vw, frame3);
        }

        if (frame3 != frame2) {
            cvReleaseImage (&frame3);
        }
        cvReleaseImage (&frame2);
        frame3 = frame2 = 0;
    }
    cvReleaseCapture (&capture);
}

void videocap(param *p) {
    int gamma_value = 1;
    if (p->filename && p->filename[0] != 0x0) {
        p->filename = 0;
    }

//    p->vw = 0;
//    p->filename = 0;
    if (p->filename) {
        p->vw = cvCreateVideoWriter (
            p->filename, 
            CV_FOURCC_PROMPT,//圧縮手法とパラメータの選択ダイアログ
//      CV_FOURCC ('P','I','M','1'), //MPEG-1
//      CV_FOURCC('D','I','B',' '), // 非圧縮
            1000/p->wait_ms, //fps
            cvSize (p->w, p->h), 
            1
        );
    }

    cvNamedWindow (p->title, CV_WINDOW_AUTOSIZE);
    cvCreateTrackbar(GAMMA_TRACKBAR_NAME, p->title, &gamma_value, 3, NULL);
    cap(p);
    cvDestroyWindow (p->title);

    if (p->vw) {
        cvReleaseVideoWriter (& p->vw);
        p->vw = 0;
    }

}

/*
int main (int argc, char **argv)
{
    char *title = "Cap";
    cvNamedWindow (title, CV_WINDOW_AUTOSIZE);
    cap(argc == 2 ? argv[1][0] - '0' : 0, title, 640, 480, 100, foo);
    cvDestroyWindow (title);
    return 0;
}
*/