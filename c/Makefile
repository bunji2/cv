TARGETS=smooth.exe \
	filter.exe \
	makeborder.exe \
	threshold.exe \
	threshold2.exe \
	videocap.exe \
	videowrite.exe \
	videocap2.exe

CFLAGS=-D_GLIBCXX_USE_CXX11_ABI=0 \
	-IC:/opencv_libs/opencv-2.4.13.5/include

LIBS=-LC:/opencv_libs/opencv-2.4.13.5/x64/mingw/staticlib \
	-static -lopencv_photo2413 \
	-lopencv_imgproc2413 \
	-lopencv_objdetect2413 \
	-lopencv_highgui2413 \
	-lopencv_core2413 \
	-lIlmImf \
	-llibjasper \
	-llibpng \
	-llibtiff \
	-lzlib \
	-llibjpeg \
	-std=c++0x \
	-lkernel32 \
	-luser32 \
	-lgdi32 \
	-lwinspool \
	-lshell32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lcomdlg32 \
	-ladvapi32 \
	-lvfw32 \
	-lstdc++

CC=gcc
RM=rm -f

.SUFFIXES: .exe .o .c

all: $(TARGETS)

clean:
	$(RM) *.o *.exe

.c.o:
	$(CC) $(CFLAGS) -c $<

.o.exe:
	$(CC) -o $@ $^ $(LIBS)

#smooth.exe: smooth.o
#	$(CC) -o $@ $^ $(LIBS)
#
#filter.exe: filter.o
#	$(CC) -o $@ $^ $(LIBS)
#
#makeborder.exe: makeborder.o
#	$(CC) -o $@ $^ $(LIBS)
#
#threshold.exe: threshold.o
#	$(CC) -o $@ $^ $(LIBS)
#