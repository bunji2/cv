/*
カメラからの画像キャプチャ cvCreateCameraCapture, cvQueryFrame
指定された番号のカメラから画像をキャプチャして表示する
http://opencv.jp/sample/video_io.html
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctype.h>

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
    int index, 
    const char* title, 
    double w, double h, 
    int wait_ms, 
    IplImage* (*func)(IplImage *), 
    CvVideoWriter *vw
) {
    CvCapture *capture = 0;
    IplImage *frame = 0, *frame2 = 0;
    // (1)コマンド引数によって指定された番号のカメラに対するキャプチャ構造体を作成する
    capture = cvCreateCameraCapture (index);
    /* この設定は，利用するカメラに依存する */

    // (2)キャプチャサイズを設定する．
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);

    // (3)カメラから画像をキャプチャする
    // キー入力があるまで繰り返し
    while (cvWaitKey (wait_ms) == -1) {
        frame = cvQueryFrame (capture);
        if (func) {
            frame2 = func(frame);
        }
        if (!frame2) {
            frame2 = frame;
        }
        cvShowImage (title, frame2);
        if (vw) {
            cvWriteFrame (vw, frame2);
        }
        if (frame2 != frame) {
            cvReleaseImage (&frame2);
        }
        frame2 = 0;
    }
    cvReleaseCapture (&capture);
}

void videocap(int cam_idx, const char* title, int w, int h, int wait_ms, int use_filter) {
    cvNamedWindow (title, CV_WINDOW_AUTOSIZE);
    if (!use_filter) {
        cap(cam_idx, title, (double)w, (double)h, wait_ms, 0, 0);    
    } else {
        cap(cam_idx, title, (double)w, (double)h, wait_ms, filter, 0);
    }
    cvDestroyWindow (title);
}

void videocap2(int cam_idx, const char* title, int w, int h, int wait_ms, int use_filter, const char* filename) {
    CvVideoWriter *vw = 0;
    if (filename) {
        vw = cvCreateVideoWriter (
            filename, 
            CV_FOURCC_PROMPT,//圧縮手法とパラメータの選択ダイアログ
//      CV_FOURCC ('P','I','M','1'), //MPEG-1
//      CV_FOURCC('D','I','B',' '), // 非圧縮
            15, //fps
            cvSize ((int) w, (int) h), 
            1
        );
    }    cvNamedWindow (title, CV_WINDOW_AUTOSIZE);
    if (!use_filter) {
        cap(cam_idx, title, (double)w, (double)h, wait_ms, 0, vw);
    } else {
        cap(cam_idx, title, (double)w, (double)h, wait_ms, filter, vw);
    }
    cvDestroyWindow (title);
    if (vw) {
        cvReleaseVideoWriter (&vw);
        vw = 0;
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