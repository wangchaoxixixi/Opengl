#version 330 core
in vec2 TexCoord;
out float y;
out float u;
out float v;
// Texture samplers
uniform sampler2D rgbTexture;
mat4 RGBtoYUV = mat4(0.257,  0.439, -0.148, 0.0,
                     0.504, -0.368, -0.291, 0.0,
                     0.098, -0.071,  0.439, 0.0,
                     0.0625, 0.500,  0.500, 1.0 );
void main()
{
    vec3 yuv;
    yuv = ( RGBtoYUV * texture(rgbTexture, TexCoord) ).rgb;
    vec3 rgb = texture(rgbTexture, TexCoord).rgb;
    rgb.r = rgb.r*256;
    rgb.g = rgb.g*256;
    rgb.b = rgb.b*256;
    y = (0.299*rgb.r + 0.587*rgb.g + 0.114*rgb.b)/256;
    u = (-0.1687*rgb.r - 0.3313*rgb.g + 0.5*rgb.b + 128)/256;
    v = (0.5*rgb.r - 0.4187*rgb.g - 0.0813*rgb.b + 128)/256;
}
