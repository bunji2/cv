package main

// #cgo pkg-config: opencv
// #include <opencv/cv.h>
// #include <opencv/highgui.h>
import "C"
import "unsafe"

import (
	"os"
	"fmt"
	"strconv"
	"time"
)

const (
	width = 640
	height = 480
	window_name = "camera"
	date_layout = "2006:01:02 15:04:05"
	path = "./haarcascade_frontalface_alt.xml"
	wait_ms = 250 // [ms]
	stop_keycode =  0x71 // stop by entering key "q"
)

var camera_index int
var font C.CvFont
var fg_color, bg_color, face_color C.CvScalar

var cvHCC *C.CvHaarClassifierCascade
var cvMStr *C.CvMemStorage

var use_face_detection bool

func main(){

	use_face_detection = true
	camera_index = 0
	if len(os.Args) > 1 {
		if tmp,err := strconv.Atoi(os.Args[1]); err != nil {
			camera_index = tmp
		}
		if len(os.Args) > 2 {
			if os.Args[2] == "OFF" {
				use_face_detection = false
			}
		}
	}

	fmt.Println("カメラ表示ツール")
	fmt.Printf("起動日時:%s\n", get_date())
	fmt.Printf("Usage %s [カメラ番号] [顔認識フラグ]\n", os.Args[0])
	fmt.Println("カメラ番号のデフォルト値=0")
	fmt.Println("顔認識フラグのデフォルト値=ON")

	fg_color = C.cvScalar(255, 255, 255, 0) // C.cvScalar(b, g, r, 0)
	bg_color = C.cvScalar(0, 0, 0, 0)

	if use_face_detection {
		cvHCC = (*C.CvHaarClassifierCascade)(C.cvLoad(C.CString(path), (*C.CvMemStorage)(nil), (*C.char)(nil), (**C.char)(nil)))
    	cvMStr = C.cvCreateMemStorage(0)
		face_color = C.cvScalar(0, 0, 255, 0)
	}

	capture := C.cvCreateCameraCapture(C.int(camera_index))
	defer C.cvReleaseCapture(&capture)

	font_face := C.int(C.CV_FONT_HERSHEY_SIMPLEX|C.CV_FONT_ITALIC)
	hscale := C.double(1.0)
	vscale := C.double(1.0)
	shear := C.double(0)
	thickness := C.int(2)
	line_type := C.int(8)
	C.cvInitFont(&font, font_face, hscale, vscale, shear, thickness, line_type)
	
	C.cvSetCaptureProperty(capture, C.CV_CAP_PROP_FRAME_WIDTH, C.double(width))
	C.cvSetCaptureProperty(capture, C.CV_CAP_PROP_FRAME_HEIGHT, C.double(height))

	C.cvNamedWindow(C.CString(window_name), C.CV_WINDOW_AUTOSIZE)
	defer C.cvDestroyWindow(C.CString(window_name))

	for C.cvWaitKey(wait_ms) != stop_keycode { 
		frame := unsafe.Pointer(C.cvQueryFrame(capture))
		if use_face_detection {
			face_detect(frame)
		}
		put_date(frame)
		C.cvShowImage(C.CString(window_name), frame)
	}
}

func put_date(frame unsafe.Pointer) {
	msg := C.CString(get_date())
	defer C.free(unsafe.Pointer(msg))

	var size C.CvSize
	var baseline C.int
	C.cvGetTextSize(msg, &font, &size, &baseline)

	x := C.int(width)-size.width
	if x < C.int(0) {
		x = C.int(0)
	}
	pos := C.cvPoint(x, size.height)

	pt1 := C.cvPoint(x, C.int(0))
	pt2 := C.cvPoint(C.int(width), size.height+baseline)
	C.cvRectangle(frame, pt1, pt2, bg_color, C.CV_FILLED, 8, 0)
	C.cvPutText(frame, msg, pos, &font, fg_color)
}

func get_date() string {
	t := time.Now()
	return t.Format(date_layout)
}


func face_detect(img unsafe.Pointer) {
//	var img2 *C.IplImage
	//C.cvCvtColor(img, unsafe.Pointer(img2), C.CV_RGB2GRAY)

    faces := C.cvHaarDetectObjects(
//        unsafe.Pointer(img2),
        unsafe.Pointer(img),
        cvHCC,
        cvMStr,
        1.11,
        3,
        0,
        C.cvSize(0, 0),
        C.cvSize(0, 0),
    )

    for i := C.int(0); i < faces.total; i++ {
        faceRect := (*C.CvRect)(unsafe.Pointer(C.cvGetSeqElem(faces, i)))    
        C.cvRectangle(
            img,
            C.cvPoint(faceRect.x, faceRect.y),
            C.cvPoint(faceRect.x+faceRect.width, faceRect.y+faceRect.height),
            face_color,
            3,
            8, //C.CV_AA,
            0,
        )
    }
}
