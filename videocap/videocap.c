/*
カメラからの画像キャプチャ cvCreateCameraCapture, cvQueryFrame
指定された番号のカメラから画像をキャプチャして表示する
http://opencv.jp/sample/video_io.html
*/

#include "videocap.h"
#include <ctype.h>

param* new_param(
    int cam_idx, 
    int w, 
    int h, 
    int wait_ms, 
    int face_detect, 
    int filter, 
    char* filename
) {
    param *p = (param*)cvAlloc(sizeof(param));
    p->cam_idx = cam_idx;
    p->title = "Cap";
    p->w = w;
    p->h = h;
    p->wait_ms = wait_ms;
    p->filter = filter;
    p->face_detect = face_detect;
    p->filename = filename;
    p->vw = 0;
    return p;
}

void release_param(param **p) {
    cvFree(p);
}

// 顔認識

#define HAARCASCADE "haarcascade_frontalface_alt.xml"
#define SCALE 1.3

typedef struct {
    CvHaarClassifierCascade* cvHCC;
    CvMemStorage* cvMStr;
}   FaceDetectCtx;

FaceDetectCtx *new_fd_ctx() {
    FaceDetectCtx *ctx = (FaceDetectCtx*)malloc(sizeof(FaceDetectCtx));
	// 正面顔検出器の読み込み
    ctx->cvHCC = (CvHaarClassifierCascade*)cvLoad(
       HAARCASCADE, NULL, NULL, NULL);

	// 検出に必要なメモリストレージを用意する
	ctx->cvMStr = cvCreateMemStorage(0);

    return ctx;
}

void release_fd_ctx(FaceDetectCtx *ctx) {
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

// ガンマ補正
// 参考URL http://imagingsolution.blog107.fc2.com/blog-entry-166.html
IplImage* cvGamma(IplImage* src, int g) {
    IplImage* dst;
    uchar LUT[256];

    //printf("#gamma = %d\n", g);
    dst = cvCreateImage(
        cvGetSize (src), 
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

IplImage *conv_ohtsu(IplImage *src_img) {
    //IplImage *dst_img = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
    IplImage *dst_img = cvCloneImage (src_img);
    cvSmooth (src_img, src_img, CV_GAUSSIAN, 5, 0, 0, 0);
    //cvThreshold (src_img, dst_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cvThreshold (src_img, dst_img, 90, 255, CV_THRESH_BINARY);
    return dst_img;
}

IplImage * filter (IplImage *src_img)
{
  IplImage *dst_img;
  IplImage *src_img_gray = 0;
  IplImage *tmp_img1, *tmp_img2, *tmp_img3;

  // (1)画像を読み込む
  if (src_img == 0)
    return 0;

  tmp_img1 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img2 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img3 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  src_img_gray = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  cvCvtColor (src_img, src_img_gray, CV_BGR2GRAY);
  dst_img = cvCloneImage (src_img);

  // (2)ガウシアンフィルタで平滑化を行う
  cvSmooth (src_img_gray, src_img_gray, CV_GAUSSIAN, 5, 0, 0, 0);

  // (3)二値化:cvThreshold
  cvThreshold (src_img_gray, tmp_img1, 90, 255, CV_THRESH_BINARY);

  // (4)二値化:cvAdaptiveThreshold
  cvAdaptiveThreshold (src_img_gray, tmp_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
    CV_THRESH_BINARY, 11, 10);

  // (5)二つの二値化画像の論理積
  cvAnd (tmp_img1, tmp_img2, tmp_img3, NULL);
  cvCvtColor (tmp_img3, dst_img, CV_GRAY2BGR);

  // (6)元画像と二値画像の論理積
  cvSmooth (src_img, src_img, CV_GAUSSIAN, 11, 0, 0, 0);
  cvAnd (dst_img, src_img, dst_img, NULL);

  cvReleaseImage (&src_img_gray);
  cvReleaseImage (&tmp_img1);
  cvReleaseImage (&tmp_img2);
  cvReleaseImage (&tmp_img3);
  return dst_img;
}

void cap (
    param *p
) {
    CvCapture *capture = 0;
    IplImage *frame = 0, *frame2 = 0, *frame3 = 0;
    FaceDetectCtx *ctx = 0;

    if (p->face_detect) {
        ctx = new_fd_ctx();
    }

    // (1)コマンド引数によって指定された番号のカメラに対するキャプチャ構造体を作成する
    capture = cvCreateCameraCapture (p->cam_idx);

    // (2)キャプチャサイズを設定する．
    /* この設定は，利用するカメラに依存する */
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, (double)(p->w));
    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, (double)(p->h));
    p->w = (int)cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH);
    p->h = (int)cvGetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT);
    printf("size = %dx%d\n", p->w, p->h);

    // (3)カメラから画像をキャプチャする
    // キー入力があるまで繰り返し
    while (cvWaitKey (p->wait_ms) == -1) {
        frame = cvQueryFrame (capture);
        if (!frame) {
            continue;
        }

        frame2 = cvGamma(frame,  cvGetTrackbarPos(GAMMA_TRACKBAR_NAME, p->title));

        switch (cvGetTrackbarPos(FILTER_TRACKBAR_NAME, p->title)) {
            case 1:
                frame3 = filter(frame2);
                break;
            case 2:
                frame3 = conv_ohtsu(frame2);
                break;

            default:

                // フィルタ処理と顔認識処理は同時に行わない
                if (p->face_detect && ctx) {
                    // 顔認識を行い顔の位置に矩形を描画
                    face_detect(ctx, frame2);
                }

                frame3 = frame2;
        }

        // 描画した画像を表示
        cvShowImage (p->title, frame3);

        if (p->vw) {
            cvWriteFrame (p->vw, frame3);
        }

        if (frame3 != frame2) {
            cvReleaseImage (&frame3);
        }
        cvReleaseImage (&frame2);
        frame3 = frame2 = 0;
    }
    cvReleaseCapture (&capture);

    if (p->face_detect && ctx) {
        release_fd_ctx(ctx);
        ctx = 0;
    }
}

void videocap(param *p) {
    int gamma_value = 1;
    if (p->filename && p->filename[0] == 0x0) {
        p->filename = 0;
    }

//    p->vw = 0;
//    p->filename = 0;
    if (p->filename) {
        p->vw = cvCreateVideoWriter (
            p->filename, 
            CV_FOURCC_PROMPT,//圧縮手法とパラメータの選択ダイアログ
//      CV_FOURCC ('P','I','M','1'), //MPEG-1
//      CV_FOURCC('D','I','B',' '), // 非圧縮
            1000/p->wait_ms, //fps
            cvSize (p->w, p->h), 
            1
        );
    }

    //cvNamedWindow (p->title, CV_WINDOW_AUTOSIZE);

    cvNamedWindow (p->title, CV_WINDOW_NORMAL);
    cvCreateTrackbar(GAMMA_TRACKBAR_NAME, p->title, &gamma_value, 3, NULL);
    cvCreateTrackbar(FILTER_TRACKBAR_NAME, p->title, &p->filter, 2, NULL);
    cap(p);
    cvDestroyWindow (p->title);

    if (p->vw) {
        cvReleaseVideoWriter (& p->vw);
        p->vw = 0;
    }

}
