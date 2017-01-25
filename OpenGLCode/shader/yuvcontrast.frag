#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform float Brightness;
uniform float Contrast;
void main()
{
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture2D(ourTexture1, TexCoord).r + Brightness;
    yuv.y = texture2D(ourTexture2, TexCoord).r - 0.5;
    yuv.z = texture2D(ourTexture3, TexCoord).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    color = vec4((rgb-vec3(0.5,0.5,0.5))*Contrast + vec3(0.5,0.5,0.5), 1.0);
}
