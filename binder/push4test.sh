FILEPATH="libs/armeabi-v7a"

adb push ${FILEPATH}/libexample_client.so /system/lib
adb push ${FILEPATH}/libexample.so /system/lib
adb push ${FILEPATH}/libexample_server.so /system/lib
adb push ${FILEPATH}/texampleServer /data/app
adb push ${FILEPATH}/texampleClient /data/app
