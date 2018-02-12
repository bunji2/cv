
TARGETS=cam.exe

GOFLAGS=build -buildmode=c-archive

xCFLAGS=`pkg-config opencv --cflags` \
	-DUSE_MAIN

xLIBS=`pkg-config opencv --libs` \
	-lstdc++


xCFLAGS=-IC:/msys64/mingw64/include/opencv \
	-IC:/msys64/mingw64/include


xLIBS=-LC:/msys64/mingw64/lib \
	-lopencv_stitching \
	-lopencv_superres \
	-lopencv_videostab \
	-lopencv_aruco \
	-lopencv_bgsegm \
	-lopencv_ccalib \
	-lopencv_dpm \
	-lopencv_face \
	-lopencv_photo \
	-lopencv_fuzzy \
	-lopencv_hdf \
	-lopencv_img_hash \
	-lopencv_line_descriptor \
	-lopencv_optflow \
	-lopencv_reg \
	-lopencv_rgbd \
	-lopencv_saliency \
	-lopencv_sfm \
	-lopencv_stereo \
	-lopencv_structured_light \
	-lopencv_phase_unwrapping \
	-lopencv_surface_matching \
	-lopencv_tracking \
	-lopencv_datasets \
	-lopencv_text \
	-lopencv_dnn \
	-lopencv_plot \
	-lopencv_xfeatures2d \
	-lopencv_shape \
	-lopencv_video \
	-lopencv_ml \
	-lopencv_ximgproc \
	-lopencv_calib3d \
	-lopencv_features2d \
	-lopencv_highgui \
	-lopencv_videoio \
	-lopencv_flann \
	-lopencv_xobjdetect \
	-lopencv_imgcodecs \
	-lopencv_objdetect \
	-lopencv_xphoto \
	-lopencv_imgproc \
	-lopencv_core \
	-lstdc++

CFLAGS=-D_GLIBCXX_USE_CXX11_ABI=0 -IC:/opencv_libs/opencv-2.4.13.5/include \
	-DUSE_MAIN

LIBS=-LC:/opencv_libs/opencv-2.4.13.5/x64/mingw/staticlib -static -lopencv_objdetect2413 -lopencv_photo2413 -lopencv_imgproc2413 -lopencv_highgui2413 -lopencv_core2413 -lIlmImf -llibjasper -llibpng -llibtiff -lzlib -llibjpeg -std=c++0x -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lvfw32 -lstdc++

GO=go
CC=gcc
CXX=g++
RM=rm -f
GREP=grep

.SUFFIXES: .o .c .cpp .exe

all: $(TARGETS)

clean:
	$(RM) *.o *.exe

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

.o.exe:
	$(CC) $^ $(LIBS) -o $@

cam.exe: cam_main.o cam.o
