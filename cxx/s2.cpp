// http://d.hatena.ne.jp/kougaku-navi/20160108/p1

#include <opencv2/opencv.hpp>

// �摜���摜�ɓ\��t����֐�
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

// �摜���摜�ɓ\��t����֐��i�T�C�Y�w����ȗ������o�[�W�����j
void paste(cv::Mat dst, cv::Mat src, int x, int y) {
	paste(dst, src, x, y, src.rows, src.cols);
}

int main() {
	cv::Mat img1 = cv::imread("life_is_action.jpg"); // �摜�̓ǂݍ���
	cv::Mat img2 = cv::imread("shukugawa.jpg");   // �摜�̓ǂݍ���

	// img1��img2��\��t����B�ʒu x=105, y=110, �T�C�Y80x80
	paste(img1, img2, 105, 110, 80, 80);

	cv::namedWindow("result");   // �E�B���h�E�̏���
	cv::imshow("result", img1);  // �摜�̕\��
	cv::waitKey();               // �L�[���͑҂�
}
