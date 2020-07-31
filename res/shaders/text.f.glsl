varying vec2 texCoord;

uniform sampler2D bayerTex;
uniform sampler2D tex;

void main(void) {
    gl_FragColor = vec4(1, 1, 1, texture2D(tex, texCoord).r);
}