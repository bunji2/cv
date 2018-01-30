/*
画像の連結 cvSetImageROI, cvResetImageROI
複数の画像を横方向に連結して1枚の画像にする
http://opencv.jp/sample/initialization.html#combine
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>

/* プロトタイプ宣言 */
IplImage *combine_image (int num, IplImage ** tmp);

/* メイン関数 */
int
main (int argc, char **argv)
{
  int i, img_num;
  IplImage **img;
  IplImage *combined_img;

  // (1)コマンド引数で指定された画像を全て読み込む
  if (argc < 2) {
    return 1;
  }
  else {
    img_num = argc - 1;
    img = (IplImage **) cvAlloc (sizeof (IplImage *) * img_num);
    for (i = 0; i < img_num; i++) {
      img[i] = cvLoadImage (argv[i + 1], CV_LOAD_IMAGE_COLOR);
      if (img[i] == 0)
        return -1;
    }
  }

  // (2)画像を連結する
  combined_img = combine_image (img_num, img);

  cvNamedWindow ("Image", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Image", combined_img);
  cvWaitKey (0);

  cvDestroyWindow ("Image");
  cvReleaseImage (&combined_img);
  for (i = 0; i < img_num; i++) {
    cvReleaseImage (&img[i]);
  }
  cvFree (&img);

  return 0;
}

/* 画像を連結する関数 */
IplImage *
combine_image (int num, IplImage ** tmp)
{
  int i;
  int width = 0, height = 0;
  IplImage *cimg;
  CvRect roi = cvRect (0, 0, 0, 0);

  // (3)与えられた各画像から，連結後の幅と高さを求める
  for (i = 0; i < num; i++) {
    width += tmp[i]->width;
    height = height < tmp[i]->height ? tmp[i]->height : height;
  }
  cimg = cvCreateImage (cvSize (width, height), IPL_DEPTH_8U, 3);
  cvZero (cimg);

  // (4)ROIを利用して各画像をコピーする
  for (i = 0; i < num; i++) {
    roi.width = tmp[i]->width;
    roi.height = tmp[i]->height;
    cvSetImageROI (cimg, roi);
    cvCopy (tmp[i], cimg, NULL);
    roi.x += roi.width;
  }
  cvResetImageROI (cimg);

  return cimg;
}