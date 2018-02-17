# Makefile for MINGW64/MSYS2 + opencv-2.4.13.5

TARGETS=s1.exe \
	s2.exe \
	s2a.exe \
	bin.exe \
	bin2.exe

CFLAGS=-D_GLIBCXX_USE_CXX11_ABI=0 \
	-IC:/opencv_libs/opencv-2.4.13.5/include

LIBS=-LC:/opencv_libs/opencv-2.4.13.5/x64/mingw/staticlib \
	-static -lopencv_photo2413 \
	-lopencv_objdetect2413 \
	-lopencv_imgproc2413 \
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
CXX=g++
RM=rm -f

.SUFFIXES: .exe .o .c .cpp

all: $(TARGETS)

clean:
	$(RM) *.o *.exe

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<

.o.exe:
	$(CC) -o $@ $^ $(LIBS)

s2a.exe: s2a_main.o s2a.o
