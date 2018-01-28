/*
平滑化 cvSmooth
ブラー，ガウシアン，メディアン，バイラテラル，の各フィルタによる平滑化
http://opencv.jp/sample/filter_and_color_conversion.html
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  int i;
  IplImage *src_img = 0, *dst_img[4];

  // (1)画像を読み込む
  printf("# loading image.\n");
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if (src_img == 0)
    exit (-1);

  for (i = 0; i < 4; i++)
    dst_img[i] = cvCloneImage (src_img);

  // (2)手法を指定して画像を平滑化
  printf("# soothing image.\n");
  cvSmooth (src_img, dst_img[0], CV_BLUR, 5, 0, 0, 0);
  cvSmooth (src_img, dst_img[1], CV_GAUSSIAN, 11, 0, 0, 0);
  cvSmooth (src_img, dst_img[2], CV_MEDIAN, 5, 0, 0, 0);
  cvSmooth (src_img, dst_img[3], CV_BILATERAL, 80, 80, 0, 0);

  // (3)処理された画像を実際に表示
  printf("# previewing image.\n");
  cvNamedWindow ("Blur", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Blur", dst_img[0]);
  cvNamedWindow ("Gaussian", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Gaussian", dst_img[1]);
  cvNamedWindow ("Median", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Median", dst_img[2]);
  cvNamedWindow ("Bilateral", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Bilateral", dst_img[3]);
  cvWaitKey (0);

  cvDestroyWindow ("Blur");
  cvDestroyWindow ("Gaussian");
  cvDestroyWindow ("Median");
  cvDestroyWindow ("Bilateral");
  cvReleaseImage (&src_img);
  for (i = 0; i < 4; i++) {
    cvReleaseImage (&dst_img[i]);
  }

  return 0;
}