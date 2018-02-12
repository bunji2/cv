#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "s2a.h"

int main() {
	IplImage *img1 = cvLoadImage("life_is_action.jpg", CV_LOAD_IMAGE_COLOR);
	IplImage *img2 = cvLoadImage("shukugawa.jpg", CV_LOAD_IMAGE_COLOR);

	// img1にimg2を貼り付ける。位置 x=105, y=110, サイズ80x80
	paste(img1, img2, 105, 110, 80, 80);

	cvNamedWindow ("xxx", CV_WINDOW_AUTOSIZE);
	cvShowImage ("xxx", img1);
	cvWaitKey (0);

	cvDestroyWindow ("xxx");
	cvReleaseImage (&img1);
	cvReleaseImage (&img2);
}
