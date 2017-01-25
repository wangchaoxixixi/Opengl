#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
float k = 2.0;//0.5<k<2
vec4 xposure(vec4 _color, float gray, float ex)
{
   float b = (4.0*ex - 1.0);
   float a = 1.0 - b;
   float f = gray*(a*gray + b);
   return f*_color;
}
void main()
{
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture2D(ourTexture1, TexCoord).r;
    yuv.y = texture2D(ourTexture2, TexCoord).r - 0.5;
    yuv.z = texture2D(ourTexture3, TexCoord).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    vec4 _dsColor = vec4(rgb,1);
    float _lum = 0.3*_dsColor.x + 0.59*_dsColor.y;
    vec4 _fColor =vec4(rgb,1);
    color = xposure(_fColor, _lum, k);
}
