attribute vec2 pos;
attribute float luminosity;

uniform mat4 projection;
uniform float time;

varying float lumin;

mat4 rotationX(in float angle)
{
    return mat4(1.0, 0, 0, 0,
        0, cos(angle), -sin(angle), 0,
        0, sin(angle), cos(angle), 0,
        0, 0, 0, 1.0);
}

mat4 rotationY(in float angle)
{
    return mat4(cos(angle), 0, sin(angle), 0,
        0, 1.0, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1.0);
}

mat4 rotationZ(in float angle)
{
    return mat4(cos(angle), -sin(angle), 0, 0,
        sin(angle), cos(angle), 0, 0,
        0, 0, 1.0, 0,
        0, 0, 0, 1.0);
}

void main()
{
    lumin = luminosity;
    gl_Position = projection * rotationZ(time) * vec4(pos, 1.0, 1.0);
}