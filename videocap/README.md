# videocap

## 概要

* カメラでキャプチャした画像を表示する。

## ビルド

```
go build
```

## コマンドオプション

```
C:\>videocap -h
Usage of videocap:
  -cam_idx int
        the camera index to use
  -file string
        the path to record video
  -filter int
        the number of filter. 0:none, 1:binary+edge, 2:ohtsu
  -size string
        the size to be captured (default "640x480")
  -wait_ms int
        the interval ms (default 100)
```