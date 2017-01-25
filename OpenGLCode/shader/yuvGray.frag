#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform vec2 TexSize;
const vec3 W = vec3(0.2125,0.7154,0.0721);

void main()
{
    vec2 pos = vec2(TexCoord.x+0.5/TexSize.x,TexCoord.y+0.5/TexSize.y);
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture2D(ourTexture1, pos).r;
    yuv.y = texture2D(ourTexture2, pos).r - 0.5;
    yuv.z = texture2D(ourTexture3, pos).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    float luminace = dot(rgb,W);
    color = vec4(luminace,luminace,luminace,1.0);
}
