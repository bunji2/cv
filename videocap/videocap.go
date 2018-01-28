package main

/*
#cgo pkg-config: opencv
#include "videocap.h"
#include <stdlib.h>
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
	default_filepath = ""
	default_size = "640x480"
	default_width = 640
	default_height = 480
	default_wait_ms = 100
	default_title = "Cam"
	default_use_filter = false
)
var cam_idx, width, height, wait_ms int
var size, filepath string
var use_filter bool

func main() {
	initialize()
	videocap(cam_idx, default_title, width, height, wait_ms, use_filter, filepath)
	fmt.Println("done.")
}

func initialize() {
	flag.StringVar(&size, "size", default_size, "the size to be captured")
	flag.BoolVar(&use_filter, "filter", default_use_filter, "the flag for using filter")
	flag.IntVar(&cam_idx, "cam_idx", default_cam_idx, "the camera index to use")
	flag.StringVar(&filepath, "file", default_filepath, "the path to record video")
	flag.IntVar(&wait_ms, "wait_ms", default_wait_ms, "the interval ms")
	flag.Parse()
	width = default_width
	height = default_height
	if w,h,ok := parse_size(size); ok {
		width = w
		height = h
	}
}

func parse_size(s string) (w, h int, ok bool) {
	fmt.Println(s)
	ok = false
	pos := strings.Index(s, "x")
	if pos<=0 {
		fmt.Println("1")
		return
	}
	tmp, err := strconv.Atoi(s[0:pos])
	if err != nil {
		fmt.Println("2")
		return
	}
	w = tmp
	tmp, err = strconv.Atoi(s[pos+1:])
	if err != nil {
		fmt.Println("3")
		return
	}
	h = tmp
	ok = true
	return
}
/*
func videocap(cam_idx int, title string, w, h, wait_ms int) {
	title_c := C.CString(title)
	defer C.free(unsafe.Pointer(title_c))
	C.videocap(C.int(cam_idx), title_c, C.int(w), C.int(h), C.int(wait_ms))
}
*/

func videocap(cam_idx int, title string, w, h, wait_ms int, use_filter bool, filepath...string) {
	filepath_ := ""
	if len(filepath)>0 {
		filepath_ = filepath[0]
	}
	use_filter_ := 0
	if use_filter {
		use_filter_ = 1
	}
	if filepath_ == "" {
		title_c := C.CString(title)
		defer C.free(unsafe.Pointer(title_c))
		C.videocap(C.int(cam_idx), title_c, C.int(w), C.int(h), C.int(wait_ms), C.int(use_filter_))
		return
	}
	title_c := C.CString(title)
	defer C.free(unsafe.Pointer(title_c))
	filepath_c := C.CString(filepath_)
	defer C.free(unsafe.Pointer(filepath_c))
	C.videocap2(C.int(cam_idx), title_c, C.int(w), C.int(h), C.int(wait_ms), C.int(use_filter_), filepath_c)
}