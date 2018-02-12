// http://d.hatena.ne.jp/kougaku-navi/20160108/p1

#include <opencv2/opencv.hpp>

// 画像を画像に貼り付ける関数
void paste(cv::Mat dst, cv::Mat src, int x, int y, int width, int height) {
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

// 画像を画像に貼り付ける関数（サイズ指定を省略したバージョン）
void paste(cv::Mat dst, cv::Mat src, int x, int y) {
	paste(dst, src, x, y, src.rows, src.cols);
}

int main() {
	cv::Mat img1 = cv::imread("life_is_action.jpg"); // 画像の読み込み
	cv::Mat img2 = cv::imread("shukugawa.jpg");   // 画像の読み込み

	// img1にimg2を貼り付ける。位置 x=105, y=110, サイズ80x80
	paste(img1, img2, 105, 110, 80, 80);

	cv::namedWindow("result");   // ウィンドウの準備
	cv::imshow("result", img1);  // 画像の表示
	cv::waitKey();               // キー入力待ち
}
