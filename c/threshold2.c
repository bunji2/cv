/*
画像の二値化 cvThreshold, cvAdaptiveThreshold
cvThreshold, cvAdaptiveThresholdを利用して，画像の二値化を行う
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>

void foo (const char* title, IplImage *src_img)
{
  IplImage *dst_img;
  IplImage *src_img_gray = 0;
  IplImage *tmp_img1, *tmp_img2, *tmp_img3;

  // (1)画像を読み込む
  if (src_img == 0)
    return;

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

  // (7)画像を表示する
  cvShowImage (title, dst_img);
  cvWaitKey (0);
  cvReleaseImage (&dst_img);
  cvReleaseImage (&src_img_gray);
  cvReleaseImage (&tmp_img1);
  cvReleaseImage (&tmp_img2);
  cvReleaseImage (&tmp_img3);
}

int main (int argc, char **argv)
{
  char * title = "Image";
  IplImage *src_img = 0;

  // (1)画像を読み込む
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_COLOR);
  if (src_img == 0) {
    return -1;
  }

  cvNamedWindow (title, CV_WINDOW_AUTOSIZE);
  foo(title, src_img);
  cvWaitKey (0);
  cvDestroyWindow (title);
  cvReleaseImage (&src_img);

  return 0;
}
