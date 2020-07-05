varying vec2 TexCoord;

uniform sampler2D texture1;

uniform sampler2D bayerTex;

float ditherBayer4x4(in vec2 pos, in float lumin)
{
    // convert to table coords
    float x = mod(pos.x, 4.0) / 4.0;
    float y = mod(pos.y, 4.0) / 4.0;

    // pick value from table
    float limit = texture2D(bayerTex, vec2(x, y)).r * 255.0;

    // pick nearest color
    return (lumin * 16.0 + limit - 8.0) / 16.0;
}

void main()
{
    // original value
    vec4 pixel    = texture2D(texture1, TexCoord);
    float average = (pixel.r + pixel.g + pixel.b) / 3.0;
    // color average
    float value = average;
    //sprites seem to look significantly worse when bayered
    //float value = ditherBayer4x4(gl_FragCoord.xy, average);
    gl_FragColor = vec4(value, value, value, pixel.a);
}
