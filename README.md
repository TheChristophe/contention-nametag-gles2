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
1. Run with sudo (due to GPIO permissions)

Conventions:
 - in glsl, texture 0 should always be the bayer matrix
