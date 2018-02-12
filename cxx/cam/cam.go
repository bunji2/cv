package main

/*
#cgo pkg-config: opencv
#include <stdlib.h>
#include "cam.h"
*/
import "C"
import (
	"unsafe"
	"flag"
	"fmt"
	"strconv"
	"strings"
)

const (
	default_cam_idx = 0
	default_size = "640x480"
	default_width = 640
	default_height = 480
	default_wait_ms = 100
	default_img_file = "out.png"
	default_disp_time = true
/*
	default_filter = 0
	default_face_detect = true
	default_filepath = ""
*/
)

var cam_idx, width, height, wait_ms int
var size, img_file string
var disp_time bool

func main() {
	fmt.Printf("Using OpenCV %s\n", opencv_version())
	fmt.Println("Enter any key to quit.")
	initialize()
	videocap()
	fmt.Println("Bye.")
}

func initialize() {
	flag.IntVar(&cam_idx, "cam_idx", default_cam_idx, "the camera index to use")
	flag.StringVar(&size, "size", default_size, "the size to be captured")
	flag.IntVar(&wait_ms, "wait_ms", default_wait_ms, "the interval ms")
	flag.StringVar(&img_file, "img_file", default_img_file, "the path to save image file")
	flag.BoolVar(&disp_time, "disp_time", default_disp_time, "the flag to display current time")
/*
	flag.IntVar(&filter, "filter", default_filter, "the number of filter. 0:none, 1:binary+edge, 2:ohtsu")
	flag.StringVar(&filepath, "file", default_filepath, "the path to record video")
	flag.BoolVar(&face_detect, "face_detect", default_face_detect, "the flag to use face detection")
*/
	flag.Parse()
	width = default_width
	height = default_height
	if w,h,ok := parse_size(size); ok {
		width = w
		height = h
	}
}

func parse_size(s string) (w, h int, ok bool) {
	//fmt.Println(s)
	ok = false
	pos := strings.Index(s, "x")
	if pos<=0 {
		//fmt.Println("1")
		return
	}
	tmp, err := strconv.Atoi(s[0:pos])
	if err != nil {
		//fmt.Println("2")
		return
	}
	w = tmp
	tmp, err = strconv.Atoi(s[pos+1:])
	if err != nil {
		//fmt.Println("3")
		return
	}
	h = tmp
	ok = true
	return
}

func opencv_version() string {
	return 	C.GoString(C.get_opencv_version())
}

func videocap() {
	img_file_c := C.CString(img_file)
	defer C.free(unsafe.Pointer(img_file_c))
	p := C.new_videocap_param();
	defer C.release_videocap_param(&p)

    C.set_videocap_param_int(p, C.CAM_IDX, C.int(cam_idx))
    C.set_videocap_param_int(p, C.WIDTH, C.int(width))
    C.set_videocap_param_int(p, C.HEIGHT, C.int(height))
    C.set_videocap_param_int(p, C.WAIT_MS, C.int(wait_ms))
    C.set_videocap_param_str(p, C.IMG_FILE, img_file_c)

	if disp_time {
		C.set_videocap_param_int(p, C.DISP_TIME, C.int(1))		
	}
    C.videocap(p);
}