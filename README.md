This is an efficient and fast QR code encoding framework for the Android platform. The encoding scheme comes from[libqrencode](https://github.com/fukuchi/libqrencode)，The project is to encapsulate libqrencode to facilitate QR code encoding on Android.

<img src="./capture/libqrencode.gif" width="360px" height="640px"/>

### How do I use libqrencode?
```
String path = "/sdcard/Movies/qr.bmp";
int code= QREncode.encode("qrsource", 400, path);
if (code >= 0) {
    Bitmap qr = BitmapFactory.decodeFile(path));
} else {
    Toast.makeText(this, "encode failed", Toast.LENGTH_SHORT).show();
}
```

### Compile & install 
It is recommended to add library as a dependency to the project for coding, and you can also extract C and JNI related code for compilation.

- android-ndk-r16b

Make sure NDK has been added to the system environment variables. Go into the library directory and execute
```
ndk-build
``` 
Finally, copy the compiled so library and QREncode into your project.


### Compile Android platform from libqrencode 

- Android Studio

Create the JNI directory, copy all the files in the libqrencode directory to the JNI directory, copy cmake / CMakeLists.txt to libqrencode and replace, configure the NDK version, and start compiling.