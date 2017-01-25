#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform sampler2D ourTextureMark;
//对比度
uniform float Contrast;
//饱和度
uniform float Saturation;
//亮度
uniform float Brightness;
vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
//老照片
uniform int isSepia;
uniform float intensity = 1.5;
mat3 Sepia_clolor_martrix = mat3(0.3588, 0.7044, 0.1368,
                                 0.2990, 0.5870, 0.1140,
                                 0.2392, 0.4696, 0.0912);
//白平衡
uniform float temperature;//0.0;-1~1
uniform float tint;//0.0;-1~1
const vec3 warmFilter = vec3(0.93, 0.54, 0.0);
const mat3 RGBtoYIQ = mat3(0.299, 0.587, 0.114, 0.596, -0.274, -0.322, 0.212, -0.523, 0.311);
const mat3 YIQtoRGB = mat3(1.0, 0.956, 0.621, 1.0, -0.272, -0.647, 1.0, -1.105, 1.702);


void main()
{
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture(ourTexture1, TexCoord).r + Brightness;
    yuv.x = (yuv.x-0.5)*Contrast +0.5;
    yuv.y = texture(ourTexture2, TexCoord).r - 0.5;
    yuv.z = texture(ourTexture3, TexCoord).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    vec4 rgb_matermark;
    rgb_matermark = texture(ourTextureMark, TexCoord);
    /*if(rgb_matermark.r-1<-0.1
            &&rgb_matermark.g-1<-0.1
            &&rgb_matermark.b-1<-0.1)
    {*/
    if(rgb_matermark.a==1){
        rgb.r = rgb_matermark.r;
        rgb.g = rgb_matermark.g;
        rgb.b = rgb_matermark.b;
    }
    //白平衡：
    //if(temperature != 5000||tint!=0.0)
   // {
        vec3 yiq = RGBtoYIQ * rgb; //adjusting tint
        yiq.b = clamp(yiq.b + tint*0.5226*0.1, -0.5226, 0.5226);
        vec3 temp = YIQtoRGB * yiq;
        vec3 processed = vec3(
           (temp.r < 0.5 ? (2.0 * temp.r * warmFilter.r) : (1.0 - 2.0 * (1.0 - temp.r) * (1.0 - warmFilter.r))),
           (temp.g < 0.5 ? (2.0 * temp.g * warmFilter.g) : (1.0 - 2.0 * (1.0 - temp.g) * (1.0 - warmFilter.g))),
           (temp.b < 0.5 ? (2.0 * temp.b * warmFilter.b) : (1.0 - 2.0 * (1.0 - temp.b) * (1.0 - warmFilter.b))));

        rgb = mix(temp, processed, temperature);//(temperature/10000.0-0.1)/0.9);
   // }
    if(isSepia == 1)//老照片:
    {
        color = vec4((intensity * rgb * Sepia_clolor_martrix) + ((1.0 - intensity) * rgb),1.0);
    }
    else
    {
        float luminance = dot(rgb, luminanceWeighting);
        vec3 greyScaleColor = vec3(luminance);
        color = vec4(mix(greyScaleColor, rgb, Saturation), 1.0);
    }

}
