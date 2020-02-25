attribute vec2 pos;
attribute vec2 texPos;

uniform mat4 projection;
uniform float time;
varying vec2 texCoord;

const float pi = 3.1415926535897932384626433832795;

void main(void) {
    // in-shader scaling factor for text
    float scale = 1.5;

    // offset by text position
    gl_Position = vec4(pos * scale, -1, 1);
    // skew text based on horizontal position and time
    float x = pos.x * pi * 2.0;
    gl_Position += vec4(0, sin(x + time) / 3.0, 0, 0);
    // base offset to top left
    gl_Position += vec4(-0.5, -0.5, 0, 0) * scale;

    texCoord = texPos;
}
