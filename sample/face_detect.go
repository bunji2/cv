// https://qiita.com/tenntenn/items/63d52c01a6339eb392f0


package main

// #cgo pkg-config: opencv
// #include <opencv/cv.h>
// #include <opencv/highgui.h>
import "C"

import (
    "flag"
    "unsafe"
)

const (
//	path = "/opencv_libs/opencv-2.4.13.5/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml"
//	path = "/opencv_libs/opencv-2.4.13.5/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml"
	path = "./haarcascade_frontalface_alt.xml"
)
var (
    filePath = flag.String("f", "lena.jpg", "file path")
)

func main() {
    flag.Parse()

    tarImg := C.cvLoadImage(C.CString(*filePath), C.CV_LOAD_IMAGE_ANYDEPTH|C.CV_LOAD_IMAGE_ANYCOLOR)

    cvHCC := (*C.CvHaarClassifierCascade)(C.cvLoad(C.CString(path), (*C.CvMemStorage)(nil), (*C.char)(nil), (**C.char)(nil)))

    cvMStr := C.cvCreateMemStorage(0)

    face := C.cvHaarDetectObjects(
        unsafe.Pointer(tarImg),
        cvHCC,
        cvMStr,
        1.11,
        3,
        0,
        C.cvSize(0, 0),
        C.cvSize(0, 0),
    )

    for i := C.int(0); i < face.total; i++ {
        faceRect := (*C.CvRect)(unsafe.Pointer(C.cvGetSeqElem(face, i)))    
        C.cvRectangle(
            unsafe.Pointer(tarImg),
            C.cvPoint(faceRect.x, faceRect.y),
            C.cvPoint(faceRect.x+faceRect.width, faceRect.y+faceRect.height),
            C.cvScalar(0, 0, 255, 0),
            3,
            C.CV_AA,
            0,
        )
    }

    C.cvNamedWindow(C.CString("face_detect"), C.CV_WINDOW_AUTOSIZE)
    C.cvShowImage(C.CString("face_detect"), unsafe.Pointer(tarImg))

    C.cvWaitKey(0)

    C.cvDestroyWindow(C.CString("face_detect"))
}
