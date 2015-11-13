The reason of this project : [REASON.md](https://github.com/bither/bither-android-lib/blob/master/REASON.md)

Related links : [Skia Issue](https://code.google.com/p/skia/issues/detail?id=3460), [Skia Discuss](https://groups.google.com/forum/#!topic/skia-discuss/p0IcyBoU8P0)

Build libjpeg-turbo-android-lib
==================

1.To get started, ensure you have the latest NDK
You must configure the path of JDK and Android SDK.

    echo "export ANDROID_HOME='Your android ndk path'" >> ~/.bash_profile

    source ~/.bash_profile


2.Build libjpeg-turbo.so

    cd ../libjpeg-turbo-android/libjpeg-turbo/jni

    ndk-build

3.You can get libjpegpi.so in 

     ../libjpeg-turbo-android/libjpeg-turbo/libs/armeabi


4.Copy libjpegpi.so to ../bither-android-lib/libjpeg-turbo-android/use-libjpeg-turbo-android/jni

     cd ../bither-android-lib/libjpeg-turbo-android/use-libjpeg-turbo-android/jni

     ndk-build

5.You can get libjpegpi.so and libpijni.so 


6.Use libjpeg-turbo in java 

     static {

        System.loadLibrary("jpegpi");
       
        System.loadLibrary("pijni");

     }
 and you must use class of "com.pi.common.util.NativeUtil"
