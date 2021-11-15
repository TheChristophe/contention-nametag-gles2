### Status

Dead - I decided to work on a [video-player based solution](https://github.com/TheChristophe/convention-nametag-video-player) instead, as otherwise it would require me to implement every different kind of primitive shapes and animations anyone could achieve with video editing software already, manually.

##Small nametag using Pi Zero W and various OLED panels

### Backend
Build with:
- `make release`
  - if you just want to build the app on a pi zero
- `make debug`
  - if you want to build and develop on the pi zero
- `make devmode`
  - if you want to develop on desktop (uses a window instead of an oled panel)

Run with:
-  `sudo ./nametag` (due to GPIO permissions)

Conventions:
 - in glsl, texture 0 should always be the bayer matrix

Useful stuff for x-builds:
 - `TOOLCHAIN=armv6-rpi-linux-gnueabihf`
 - `TOOLDIR=/home/chris/x-tools/${TOOLCHAIN}`
 - configure (freetype?): `CC=${TOOLDIR}/bin/${TOOLCHAIN}-gcc CXX=${TOOLDIR}/bin/${TOOLCHAIN}-g++ ./configure --prefix=/home/chris/x-tools/${TOOLCHAIN}/${TOOLCHAIN}/sysroot/usr $@`
 - ffmpeg: `CC={$TOOLDIR}/bin/{$TOOLCHAIN}-gcc CXX={$TOOLDIR}/bin/{$TOOLCHAIN}-g++ ./configure --prefix=/home/chris/x-tools/{$TOOLCHAIN}/{$TOOLCHAIN}/sysroot/usr --arch=armel --target-os=linux --enable-cross-compile --cross-prefix={$TOOLDIR}/bin/{$TOOLCHAIN}-`0
 - ./cross-cmake.sh
 - uSockets: `CC=/home/chris/x-tools/armv6-rpi-linux-gnueabihf/bin/armv6-rpi-linux-gnueabihf-gcc CXX=/home/chris/x-tools/armv6-rpi-linux-gnueabihf/bin/armv6-rpi-linux-gnueabihf-g++ PREFIX=/home/chris/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/ CXXFLAGS=-I/home/chris/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot/usr/include/ LDFLAGS=-L/home/chris/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot/usr/lib make`
