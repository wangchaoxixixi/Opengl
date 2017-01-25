#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform vec2 TexSize;


float _waterPower = 40.0;
float _quatLevel = 5.0;
vec4 quant(vec4 _cl, float n)
{
   _cl.x = floor(_cl.x*255.0/n)*n/255.0;
   _cl.y = floor(_cl.y*255.0/n)*n/255.0;
   _cl.z = floor(_cl.z*255.0/n)*n/255.0;
   return _cl;
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
   vec4 noiseColor = _waterPower * vec4(rgb, 1);
   vec2 newUV =vec2 (TexCoord.x + noiseColor.x/TexSize.x,TexCoord.y + noiseColor.y/TexSize.y);
   yuv.x = texture2D(ourTexture1, newUV).r;
   yuv.y = texture2D(ourTexture2, newUV).r - 0.5;
   yuv.z = texture2D(ourTexture3, newUV).r - 0.5;
   rgb = mat3( 1,       1,         1,
               0,       -0.39465,  2.03211,
               1.13983, -0.58060,  0) * yuv;
   vec4 _fColor = vec4(rgb,1.0);
   color = quant(_fColor, 255.0/pow(2,_quatLevel));
}                                                ;
