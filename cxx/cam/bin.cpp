#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int
main(int argc, char *argv[])
{
  //cv::Mat gray_img = cv::imread("../../image/lenna.png", 0);
  cv::Mat gray_img = cv::imread("corridor.jpg", 0);
  if(gray_img.empty()) return -1;
  
  // 固定の閾値処理
  cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
  // 入力画像，出力画像，閾値，maxVal，閾値処理手法
  cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
  cv::threshold(gray_img, bininv_img, 0, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
  cv::threshold(gray_img, trunc_img, 0, 255, cv::THRESH_TRUNC|cv::THRESH_OTSU);
  cv::threshold(gray_img, tozero_img, 0, 255, cv::THRESH_TOZERO|cv::THRESH_OTSU);
  cv::threshold(gray_img, tozeroinv_img, 0, 255, cv::THRESH_TOZERO_INV|cv::THRESH_OTSU);

  // 適応的な閾値処理
  cv::Mat adaptive_img;
  // 入力画像，出力画像，maxVal，閾値決定手法，閾値処理手法，blockSize，C
  cv::adaptiveThreshold(gray_img, adaptive_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 8);

  // 結果画像表示
  cv::namedWindow("Binary", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("Binary Inv", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("Trunc", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("ToZero", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("ToZero Inv", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("Adaptive", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow("Binary", bin_img);
  cv::imshow("Binary Inv", bininv_img);
  cv::imshow("Trunc", trunc_img);
  cv::imshow("ToZero", tozero_img);
  cv::imshow("ToZero Inv", tozeroinv_img);
  cv::imshow("Adaptive", adaptive_img);
  cv::waitKey(0);
}