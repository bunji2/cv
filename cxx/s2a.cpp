// http://d.hatena.ne.jp/kougaku-navi/20160108/p1

#include <opencv2/opencv.hpp>
#include "s2a.h"

using namespace cv;

// ‰æ‘œ‚ð‰æ‘œ‚É“\‚è•t‚¯‚éŠÖ”
void paste(IplImage *dst_img, IplImage *src_img, int x, int y, int width, int height) {
	Mat dst(dst_img);
	Mat src(src_img);
	Mat resized_img;
	resize(src, resized_img, Size(width, height));

	if (x >= dst.cols || y >= dst.rows) return;
	int w = (x >= 0) ? std::min(dst.cols - x, resized_img.cols) : std::min(std::max(resized_img.cols + x, 0), dst.cols);
	int h = (y >= 0) ? std::min(dst.rows - y, resized_img.rows) : std::min(std::max(resized_img.rows + y, 0), dst.rows);
	int u = (x >= 0) ? 0 : std::min(-x, resized_img.cols - 1);
	int v = (y >= 0) ? 0 : std::min(-y, resized_img.rows - 1);
	int px = std::max(x, 0);
	int py = std::max(y, 0);

	Mat roi_dst = dst(Rect(px, py, w, h));
	Mat roi_resized = resized_img(Rect(u, v, w, h));
	roi_resized.copyTo(roi_dst);
}

/*
// ‰æ‘œ‚ð‰æ‘œ‚É“\‚è•t‚¯‚éŠÖ”
void paste(IplImage *dst_img, IplImage *src_img, int x, int y, int width, int height) {
	cv::Mat dst(dst_img);
	cv::Mat src(src_img);
	cv::Mat resized_img;
	cv::resize(src, resized_img, cv::Size(width, height));

	if (x >= dst.cols || y >= dst.rows) return;
	int w = (x >= 0) ? std::min(dst.cols - x, resized_img.cols) : std::min(std::max(resized_img.cols + x, 0), dst.cols);
	int h = (y >= 0) ? std::min(dst.rows - y, resized_img.rows) : std::min(std::max(resized_img.rows + y, 0), dst.rows);
	int u = (x >= 0) ? 0 : std::min(-x, resized_img.cols - 1);
	int v = (y >= 0) ? 0 : std::min(-y, resized_img.rows - 1);
	int px = std::max(x, 0);
	int py = std::max(y, 0);

	cv::Mat roi_dst = dst(cv::Rect(px, py, w, h));
	cv::Mat roi_resized = resized_img(cv::Rect(u, v, w, h));
	roi_resized.copyTo(roi_dst);
}
*/