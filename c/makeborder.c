/*
境界線の作成 cvCopyMakeBorder
画像のコピーと境界の作成
http://opencv.jp/sample/filter_and_color_conversion.html
*/
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main (int argc, char **argv)
{
  int i, offset = 30;
  IplImage *src_img = 0, *dst_img[2];

  // (1)画像の読み込み
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if (src_img == 0)
    exit (-1);

  for (i = 0; i < 2; i++)
    dst_img[i] =
      cvCreateImage (cvSize (src_img->width + offset * 2, src_img->height + offset * 2), src_img->depth,
                     src_img->nChannels);

  // (2)境界線の作成
  cvCopyMakeBorder (src_img, dst_img[0], cvPoint (offset, offset), IPL_BORDER_REPLICATE, cvScalarAll(0));
  cvCopyMakeBorder (src_img, dst_img[1], cvPoint (offset, offset), IPL_BORDER_CONSTANT, CV_RGB (255, 0, 0));

  // (3)画像の表示
  cvNamedWindow ("Border_replicate", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Border_replicate", dst_img[0]);
  cvNamedWindow ("Border_constant", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Border_constant", dst_img[1]);
  cvWaitKey (0);

  cvDestroyWindow ("Border_replicate");
  cvDestroyWindow ("Border_constant");
  cvReleaseImage (&src_img);
  for (i = 0; i < 2; i++) {
    cvReleaseImage (&dst_img[i]);
  }

  return 0;
}
