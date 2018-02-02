/*
画像の二値化（大津の手法） cvThreshold
大津の手法を利用して閾値を決定し，画像の二値化を行う

*/

#include <opencv/cv.h>
#include <opencv/highgui.h>

// 二値化
IplImage *conv_ohtsu(IplImage *src_img) {
    IplImage *dst_img = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
    cvSmooth (src_img, src_img, CV_GAUSSIAN, 5, 0, 0, 0);
    cvThreshold (src_img, dst_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    return dst_img;
}

int main (int argc, char **argv)
{
  IplImage *src_img = 0, *dst_img;

  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if (src_img == 0)
    return -1;

  dst_img = conv_ohtsu(src_img);

  cvNamedWindow ("Threshold", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Threshold", dst_img);
  cvWaitKey (0);

  cvDestroyWindow ("Threshold");
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);

  return 0;
}