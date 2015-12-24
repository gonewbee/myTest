## mediainfo查看视频信息
$ mediainfo test.h264

## ffplay播放视频文件
$ ffplay test.h264

## 使用gst-launch的playbin播放
$ gst-launch-1.0 playbin uri=file://`pwd`/test.h264

## 使用gst-launch的decodebin和videoconvert播放视频
$ gst-launch-1.0 filesrc location=`pwd`/test.h264 ! decodebin name=videodecoder ! videoconvert ! autovideosink
