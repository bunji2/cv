/*
ユーザ定義フィルタ cvFilter2D
ユーザが定義したカーネルによるフィルタリング
http://opencv.jp/sample/filter_and_color_conversion.html
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
  IplImage *src_img = 0, *dst_img;
  float data[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };
  CvMat kernel = cvMat (1, 21, CV_32F, data);

  // (1)画像の読み込み
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if (src_img == 0)
    exit (-1);

  dst_img = cvCreateImage (cvGetSize (src_img), src_img->depth, src_img->nChannels);

  // (2)カーネルの正規化と，フィルタ処理
  cvNormalize (&kernel, &kernel, 1.0, 0, CV_L1, NULL);
  cvFilter2D (src_img, dst_img, &kernel, cvPoint (0, 0));

  // (3)処理画像の表示
  cvNamedWindow ("Filter2D", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Filter2D", dst_img);
  cvWaitKey (0);

  cvDestroyWindow ("Filter2D");
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);

  return 0;
}