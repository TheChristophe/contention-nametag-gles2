##Small nametag using Pi Zero W and various OLED panels

### Backend
Build with:
  1. make

~~Make a cup of tea (compilation is slow)~~

Run with:
1. Run with sudo

Conventions:
 - in glsl, texture 0 should always be the bayer matrix

### Frontend
Setup with:
1. python -m venv venv
2. . ./venv/bin/activate
3. pip install -r requirements.txt

Run with:
1. . ./venv/bin/activate
1. flask run --host=0.0.0.0
