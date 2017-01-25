#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform vec2 TexSize;

vec2 mosaicSize = vec2(32,32);
void main()
{
   vec2 intXY = vec2(TexCoord.x*TexSize.x, TexCoord.y*TexSize.y);
   vec2 XYMosaic = vec2(floor(intXY.x/mosaicSize.x)*mosaicSize.x,floor(intXY.y/mosaicSize.y)*mosaicSize.y)+ 0.5*mosaicSize;
   vec2 delXY = XYMosaic - intXY;
   float delL = length(delXY);
   vec2 UVMosaic = vec2(XYMosaic.x/TexSize.x,XYMosaic.y/TexSize.y);
   vec3 yuv;
   vec3 rgb;
   yuv.x = texture2D(ourTexture1, UVMosaic).r;
   yuv.y = texture2D(ourTexture2, UVMosaic).r - 0.5;
   yuv.z = texture2D(ourTexture3, UVMosaic).r - 0.5;
   rgb = mat3( 1,       1,         1,
               0,       -0.39465,  2.03211,
               1.13983, -0.58060,  0) * yuv;
   vec3 finalColor;
    if(delL< 0.5*mosaicSize.x)
        finalColor = rgb;
    else{
        yuv.x = texture2D(ourTexture1, TexCoord).r;
        yuv.y = texture2D(ourTexture2, TexCoord).r - 0.5;
        yuv.z = texture2D(ourTexture3, TexCoord).r - 0.5;
        rgb = mat3( 1,       1,         1,
                    0,       -0.39465,  2.03211,
                    1.13983, -0.58060,  0) * yuv;
        finalColor = rgb;
    }
    color = vec4(finalColor,1.0);
}
