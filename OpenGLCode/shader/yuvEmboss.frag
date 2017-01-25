#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform vec2 TexSize;

void main()
{
   vec2 tex =TexCoord;
   vec2 upLeftUV = vec2(tex.x-1.0/TexSize.x,tex.y-1.0/TexSize.y);
   vec3 yuv;
   vec3 rgb;
   yuv.x = texture2D(ourTexture1, TexCoord).r;
   yuv.y = texture2D(ourTexture2, TexCoord).r - 0.5;
   yuv.z = texture2D(ourTexture3, TexCoord).r - 0.5;
   rgb = mat3( 1,       1,         1,
               0,       -0.39465,  2.03211,
               1.13983, -0.58060,  0) * yuv;

   vec4 curColor = vec4(rgb,1.0);

   yuv.x = texture2D(ourTexture1, upLeftUV).r;
   yuv.y = texture2D(ourTexture2, upLeftUV).r - 0.5;
   yuv.z = texture2D(ourTexture3, upLeftUV).r - 0.5;
   rgb = mat3( 1,       1,         1,
               0,       -0.39465,  2.03211,
               1.13983, -0.58060,  0) * yuv;
   vec4 upLeftColor = vec4(rgb,1.0);
   vec4 delColor = curColor - upLeftColor;
   float h = 0.3*delColor.x + 0.59*delColor.y + 0.11*delColor.z;
   vec4 bkColor = vec4(0.5, 0.5, 0.5, 1.0);
   color = vec4(h,h,h,0.0) +bkColor;
}
