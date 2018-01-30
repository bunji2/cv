/*
画像のサイズ変更 cvResize
指定した出力画像サイズに合うように、入力画像のサイズを変更し出力する．
画像のファイル保存 cvSaveImage
指定した出力画像をファイルに保存する
*/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
  int new_width = 0, new_height = 0;
  IplImage *src_img = 0, *dst_img = 0;

  // (1)引数の処理
  if (argc < 5) {
    printf("Usage: %s src_image new_width new_height dst_image\n", argv[0]);
    return -1;
  }

  src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  new_width = atoi(argv[2]);
  new_height = atoi(argv[3]);

  if ((new_width <= 0 && new_height <= 0) || src_img == 0)
    return -1;

  // 出力用画像のサイズを計算
  if (new_width <= 0) {
    new_width = src_img->width * new_height / src_img->height;
  }
  if (new_height <= 0) {
    new_height = src_img->height * new_width / src_img->width;
  }

  // (2)出力用画像領域の確保を行なう
  dst_img = cvCreateImage (cvSize (new_width, new_height), src_img->depth, src_img->nChannels);

  // (3)画像のサイズ変更を行う
  cvResize (src_img, dst_img, CV_INTER_NN);

  // (4)結果を保存する
  cvSaveImage(argv[4], dst_img, 0);
  /*
  cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("dst", CV_WINDOW_AUTOSIZE);
  cvShowImage ("src", src_img);
  cvShowImage ("dst", dst_img);
  cvWaitKey (0);

  cvDestroyWindow ("src");
  cvDestroyWindow ("dst");
  */
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);

  return 1;
}
