attribute vec2 pos;
attribute vec2 texCoord;

varying vec2 TexCoord;

uniform mat4 projection;
uniform vec2 offset;
uniform vec2 scale;
uniform float time;

void main(void)
{
    gl_Position = projection * (vec4((vec2(-0.5, -0.5) + pos) * scale, 1.0, 1.0) + vec4(offset, 0.0, 0.0) + vec4(0.0, 6.0 * sin(time / 8.0), 0.0, 0.0));
    TexCoord    = vec2(texCoord.x, texCoord.y);
}
