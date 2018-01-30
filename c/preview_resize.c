/*
画像のサイズ変更 cvResize
指定した出力画像サイズに合うように、入力画像のサイズを変更し出力する．
http://opencv.jp/sample/sampling_and_geometricaltransforms.html#resize
*/
#include <opencv/cv.h>
#include <opencv/highgui.h>

int
main (int argc, char **argv)
{
  IplImage *src_img = 0, *dst_img1 = 0, *dst_img2 = 0;

  // (1)画像を読み込む
  if (argc >= 2)
    src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if (src_img == 0)
    return -1;

  // (2)出力用画像領域の確保を行なう
  dst_img1 = cvCreateImage (cvSize (src_img->width / 2, src_img->height / 2), src_img->depth, src_img->nChannels);
  dst_img2 = cvCreateImage (cvSize (src_img->width * 2, src_img->height * 2), src_img->depth, src_img->nChannels);

  // (3)画像のサイズ変更を行う
  cvResize (src_img, dst_img1, CV_INTER_NN);
  cvResize (src_img, dst_img2, CV_INTER_NN);

  // (4)結果を表示する
  cvNamedWindow ("src", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("dst1", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("dst2", CV_WINDOW_AUTOSIZE);
  cvShowImage ("src", src_img);
  cvShowImage ("dst1", dst_img1);
  cvShowImage ("dst2", dst_img2);
  cvWaitKey (0);

  cvDestroyWindow ("src");
  cvDestroyWindow ("dst1");
  cvDestroyWindow ("dst2");
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img1);
  cvReleaseImage (&dst_img2);

  return 1;
}
