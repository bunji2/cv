package cv

// #cgo pkg-config: opencv
// #include <opencv/cv.h>
// #include <opencv/highgui.h>
import "C"
import "unsafe"

type ScalarT C.CvScalar
func Scalar(b, g, r, a int) ScalarT {
	return C.cvScalar(b, g, r, a)
}

type FontT C.CvFont

type CaptureT C.CvCapture

/*
CaptureFromCAM creates VideoCapture constructors.

  C: CvCapture* cvCaptureFromCAM(int device)

  param: device - id of the opened video capturing device (i.e. a camera index).
  If there is a single camera connected, just pass 0.
*/
func CaptureFromCAM(device int) *CaptureT {
	return C.cvCaptureFromCAM(C.int(device))
}

/*
ReleaseCapture closes video file or capturing device and deallocates memory 
and clears *capture pointer.

  C: void cvReleaseCapture(CvCapture** capture)
*/
func ReleaseCapture(capture **CaptureT) {
	C.cvReleaseCapture(unsafe.Pointer(capture))
}

/*
GrabFrame Grabs the next frame from video file or capturing device.

  C: int cvGrabFrame(CvCapture* capture)
*/
func GrabFrame(capture *CaptureT) int {
	return int(C.cvGrabFrame(unsafe.Pointer(capture)))
}

type IplImageT C.IplImage
type MatT C.CvMat

/*

RetrieveFrame ecodes and returns the grabbed video frame. If no frames has 
been grabbed (camera has been disconnected, or there are no more frames in 
video file), the methods return false and the functions return NULL pointer.

  C: IplImage* cvRetrieveFrame(CvCapture* capture, int streamIdx=0 )

  Note OpenCV 1.x functions cvRetrieveFrame and cv.RetrieveFrame return image
  stored inside the video capturing structure. It is not allowed to modify or
  release the image! You can copy the frame using cvCloneImage() and then do
  whatever you want with the copy.

*/
func RetrieveFrame(capture *CaptureT) *IplImageT {
	return C.cvRetrieveFrame(unsafe.Pointer(capture), C.int(0))
}

/*
  GetCaptureProperty returns the specified VideoCapture property

  C: double cvGetCaptureProperty(CvCapture* capture, int property_id)
  param: propId – Property identifier.
*/
func GetCaptureProperty(capture *CaptureT, property_id int) float {
	return float(C.cvGetCaptureProperty(unsafe.Pointer(capture), C.int(property_id)))
}

/*
  SetCaptureProperty sets a property in the VideoCapture.

  C: int cvSetCaptureProperty(CvCapture* capture, int property_id, double value)
  param: propId – Property identifier.  
*/
func SetCaptureProperty(capture *CaptureT, property_id int, value float) int {
	return int(C.cvSetCaptureProperty(unsafe.Pointer(capture), C.int(property_id), C.double(value)))
}


/*
DecodeImage reads an image from a buffer in memory.

  C: IplImage* cvDecodeImage(const CvMat* buf, int iscolor=CV_LOAD_IMAGE_COLOR)
  param: buf – Input array or vector of bytes.
  param: iscolor – The same flags as in LoadImage.
*/
func DecodeImage(buf *MatT, iscolor...int) *IplImageT {
	iscolor_ := C.CV_LOAD_IMAGE_COLOR
	if len(iscolor)>0 {
		iscolor_ = C.int(iscolor[0])
	}
	return cvDecodeImage(unsafe.Pointer(buf), iscolor_)
}

/*
DecodeImageM reads an image from a buffer in memory.

  C: CvMat* cvDecodeImageM(const CvMat* buf, int iscolor=CV_LOAD_IMAGE_COLOR)
  param: buf – Input array or vector of bytes.
  param: iscolor – The same flags as in LoadImage.
*/
func DecodeImageM(buf *MatT, iscolor...int) *MatT {
	iscolor_ := C.CV_LOAD_IMAGE_COLOR
	if len(iscolor)>0 {
		iscolor_ = C.int(iscolor[0])
	}
	return cvDecodeImageM(unsafe.Pointer(buf), iscolor_)
}

/*
EncodeImage encodes an image into a memory buffer.

  C: CvMat* cvEncodeImage(const char* ext, const CvArr* image, const int* params=0 )
  param: ext – File extension that defines the output format.
  param: img – Image to be written.
  param: params – Format-specific parameters.
*/
func EncodeImage(ext string, image *IplImageT, params...int) *MatT{
	params_ := C.int(0)
	if len(params)>0 {
		params_ = C.int(params[0])
	}
	return C.cvEncodeImage(C.CString(ext), unsafe.Pointer(image), &params_)
}

/*
SaveImage saves an image to a specified file.

  C: int cvSaveImage(const char* filename, const CvArr* image, const int* params=0 )
  param: filename – Name of the file.
  param: image – Image to be saved.
  param: params – Format-specific save parameters encoded as pairs 
  paramId_1, paramValue_1, paramId_2, paramValue_2, ... . The following 
  parameters are currently supported:
  For JPEG, it can be a quality ( CV_IMWRITE_JPEG_QUALITY ) from 0 to 100 
   (the higher is the better). Default value is 95.
  For PNG, it can be the compression level ( CV_IMWRITE_PNG_COMPRESSION ) 
  from 0 to 9. A higher value means a smaller size and longer compression 
   time. Default value is 3.
  For PPM, PGM, or PBM, it can be a binary format flag ( CV_IMWRITE_PXM_BINARY 
   ), 0 or 1. Default value is 1.
*/
func SaveImage(filename string, image *IplImageT, params...int) int {
	params_ := C.int(0)
	if len(params)>0 {
		params_ = C.int(params[0])
	}
	return C.cvSaveImage(C.CString(filename), unsafe.Pointer(image), params_)
}


/*
LoadImage loads an image from a file.

  C: IplImage* cvLoadImage(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR )
  param: filename – Name of file to be loaded.
  param: iscolor – Flags specifying the color type of a loaded image:
   CV_LOAD_IMAGE_ANYDEPTH - If set, return 16-bit/32-bit image when the input has the corresponding depth, otherwise convert it to 8-bit.
   CV_LOAD_IMAGE_COLOR - If set, always convert image to the color one
   CV_LOAD_IMAGE_GRAYSCALE - If set, always convert image to the grayscale one
   >0 Return a 3-channel color image.
   =0 Return a grayscale image.
   <0 Return the loaded image as is (with alpha channel).
*/
func LoadImage(filename string, iscolor...int) *IplImageT{
	iscolor_ := C.CV_LOAD_IMAGE_COLOR 
	if len(iscolor)>0 {
		iscoilor_ = iscolor[0]
	}
	return C.cvLoadImage(C.CString(filename), C.int(iscolor_))
}

type TrackbarCallback C.TrackbarCallback
/*
Creates a trackbar and attaches it to the specified window.

  C: int cvCreateTrackbar(const char* trackbar_name, const char* window_name, int* value, int count, CvTrackbarCallback on_change=NULL )
  param: trackbar_name – Name of the created trackbar.
  param: window_name – Name of the window that will be used as a parent of the created trackbar.
  param: value – Optional pointer to an integer variable whose value reflects the position of the slider. Upon creation, the slider position is defined by this variable.
  param: count – Maximal position of the slider. The minimal position is always 0.
  param: onChange – Pointer to the function to be called every time the slider changes position. This function should be prototyped as void Foo(int,void*); , where the first parameter is the trackbar position and the second parameter is the user data (see the next parameter). If the callback is the NULL pointer, no callbacks are called, but only value is updated.
*/
func CreateTrackbar(trackbar_name string, window_name string, value *C.int, count int, on_change... CvTrackbarCallback) int {
	on_change_ := TrackbarCallback(nil)
	if len(on_change)>0 {
		on_change_ = on_change[0]
	}
	return C.int(C.cvCreateTrackbar(C.CString(trackbar_name), C.CString(window_name), value, C.int(count), on_change_))

}



/*


var cvHCC *C.CvHaarClassifierCascade
var cvMStr *C.CvMemStorage

var use_face_detection bool

func main(){

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
*/
