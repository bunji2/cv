/*
face_detect.c
カメラで画像をキャプチャし、顔認識するサンプル。
トラックバーでガンマ補正するサンプル。

参考サイト
http://www.aianet.ne.jp/~asada/prog_doc/opencv/opencv_obj_det_img.htm
*/

#include "opencv/cv.h"
#include "opencv/highgui.h"

#define SCALE 1.3
#define CAM_IDX 1

typedef struct {
    CvHaarClassifierCascade* cvHCC;
    CvMemStorage* cvMStr;
}   FaceDetectCtx;

FaceDetectCtx *new_ctx() {
    FaceDetectCtx *ctx = (FaceDetectCtx*)malloc(sizeof(FaceDetectCtx));
	// 正面顔検出器の読み込み
    ctx->cvHCC = (CvHaarClassifierCascade*)cvLoad(
        "haarcascade_frontalface_alt.xml", NULL, NULL, NULL);

	// 検出に必要なメモリストレージを用意する
	ctx->cvMStr = cvCreateMemStorage(0);

    return ctx;
}

void release_ctx(FaceDetectCtx *ctx) {
    // 生成したメモリストレージを解放
	cvReleaseMemStorage(&ctx->cvMStr);
	// カスケード識別器の解放
	cvReleaseHaarClassifierCascade(&ctx->cvHCC);
    free(ctx);
}

void face_detect(FaceDetectCtx *ctx, IplImage *frame) {
	// 顔検出した矩形領域のリストを格納する変数
	CvSeq* faces;
    // 顔検出した矩形のオフセットとサイズ用の変数
    CvRect* faceRect;
    // グレイスケール化した画像と、顔検出の対象となる画像用の変数
	IplImage *gray = 0, *detect_frame = 0;

	// 読み込んだ画像のグレースケール化
	gray = cvCreateImage(
        cvSize(frame->width, frame->height), 
        IPL_DEPTH_8U, 
        1);
	cvCvtColor(frame, gray, CV_BGR2GRAY);

	// グレースケール画像のヒストグラムを均一化
	detect_frame = cvCreateImage(
        cvSize((frame->width / SCALE), (frame->height / SCALE)), 
        IPL_DEPTH_8U, 
        1);
	cvResize(gray, detect_frame, CV_INTER_LINEAR);
	cvEqualizeHist(detect_frame, detect_frame);

	// 画像中から検出対象の情報を取得する
	faces = cvHaarDetectObjects(
        detect_frame, 
        ctx->cvHCC, 
        ctx->cvMStr, 
        1.1, 
        2, 
        CV_HAAR_DO_CANNY_PRUNING, 
        cvSize(30, 30), 
        cvSize(0,0) );

    // 検出したすべての顔の矩形領域について処理
	for (int i = 0; i < faces->total; i++) {
		// 検出情報から顔の位置情報を取得
		faceRect = (CvRect*)cvGetSeqElem(faces, i);

		// 取得した顔の位置情報に基づき、矩形描画を行う
		cvRectangle(frame,
			cvPoint(faceRect->x * SCALE, faceRect->y * SCALE),
			cvPoint((faceRect->x + faceRect->width) * SCALE,
            (faceRect->y + faceRect->height) * SCALE),
			CV_RGB(255, 0 ,0),
			3,
            CV_AA,
            0
        );
	}
    cvReleaseImage(&gray);
    cvReleaseImage(&detect_frame);
}

int gamma_value = 1;

void set_gamma_value(int x) {
    gamma_value = x;
}

// ガンマ補正
// 参考URL http://imagingsolution.blog107.fc2.com/blog-entry-166.html
IplImage* cvGamma(IplImage* src, int g) {
    IplImage* dst;
    uchar LUT[256];

    //printf("#gamma = %d\n", g);
    dst = cvCreateImage(
        cvSize(src->width, src->height), 
        src->depth, 
        src->nChannels);
	cvCopy(src, dst, NULL);

    //ガンマ補正テーブルの作成  
    for (int i = 0; i < 256; i++){  
        LUT[i] = (int)(pow((double)i / 255.0, 1.0 / (double)g) * 255.0);  
    }

    //CvMatへ変換  
    CvMat lut_mat = cvMat(1, 256, CV_8UC1, LUT);  

    //ルックアップテーブル変換  
    cvLUT(src, dst, &lut_mat);

    return dst;
}

// メインループ

int main_face_detect(const char* title, int cam_idx, int wait_ms, int height, int width) {
	// ビデオキャプチャ構造体
	CvCapture *capture = 0;
	// フレーム単位データ
	IplImage *frame = 0;
	// フレーム単位データコピー用
	IplImage *frame_copy = 0;
	IplImage *frame_copy2 = 0;
	// 縦横サイズ

    FaceDetectCtx *ctx = new_ctx();
	
	// 0番号のカメラに対するキャプチャ構造体を生成するる
	capture = cvCreateCameraCapture (cam_idx);
	
	// キャプチャのサイズを設定する。
    // ただし、この設定はキャプチャを行うカメラに依存するので、設定通りにならないこともある。
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, (double)width);
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
	width = (int)cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH);
	height = (int)cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT);

    // 表示用ウィンドウの作成
	cvNamedWindow (title, CV_WINDOW_AUTOSIZE);
	//cvCreateTrackbar("gamma", title, &gamma_value, 100, set_gamma_value);
	cvCreateTrackbar("gamma", title, &gamma_value, 5, NULL);

    // loop will be stopped when you hit any key.  
	while (cvWaitKey(wait_ms) < 0) {
		frame = cvQueryFrame (capture);

		// フレームコピー用イメージ生成
		frame_copy = cvCreateImage(
            cvSize(frame->width, frame->height), 
            IPL_DEPTH_8U, 
            frame->nChannels);
		if(frame->origin == IPL_ORIGIN_TL) {
			cvCopy(frame, frame_copy, NULL);
		} else {
			cvFlip(frame, frame_copy, 0);
		}

        // 顔位置に矩形描画を施した画像を作成
        face_detect(ctx, frame_copy);

        //printf("gamma = %d\n", cvGetTrackbarPos("gamma", title));

        frame_copy2 = cvGamma(frame_copy, cvGetTrackbarPos("gamma", title));
		// 画像を表示
		cvShowImage (title, frame_copy2);

        // 画像ヘッダと画像データの解放
        cvReleaseImage(&frame_copy);
        cvReleaseImage(&frame_copy2);
	}

	// ビデオキャプチャ構造体の解放
	cvReleaseCapture (&capture);
	
	// ウィンドウの破棄
	cvDestroyWindow(title);

    if (ctx) {
        release_ctx(ctx);
        ctx = 0;
    }
}

int main(int argc, char* argv[]) {
	main_face_detect("capture_face_detect", CAM_IDX, 100, 640, 480);
	return 0;
}