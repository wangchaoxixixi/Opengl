#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform vec2 TexSize;
vec4 dip_filter(mat3 _filter, sampler2D ourTexture1, sampler2D ourTexture2, sampler2D ourTexture3, vec2 _xy, vec2 texSize)
{
   mat3 _filter_pos_delta_x=mat3(vec3(-1.0, 0.0, 1.0), vec3(0.0, 0.0 ,1.0) ,vec3(1.0,0.0,1.0));
   mat3 _filter_pos_delta_y=mat3(vec3(-1.0,-1.0,-1.0),vec3(-1.0,0.0,0.0),vec3(-1.0,1.0,1.0));
   vec4 final_color = vec4(0.0, 0.0, 0.0, 0.0);
   for(int i = 0; i<3; i++)
   {
       for(int j = 0; j<3; j++)
       {
           vec2 _xy_new = vec2(_xy.x + _filter_pos_delta_x[i][j], _xy.y + _filter_pos_delta_y[i][j]);
           vec2 _uv_new = vec2(_xy_new.x/texSize.x, _xy_new.y/texSize.y);
           vec3 yuv;
           vec3 rgb;
           yuv.x = texture2D(ourTexture1, _uv_new).r;
           yuv.y = texture2D(ourTexture2, _uv_new).r - 0.5;
           yuv.z = texture2D(ourTexture3, _uv_new).r - 0.5;
           rgb = mat3( 1,       1,         1,
                       0,       -0.39465,  2.03211,
                       1.13983, -0.58060,  0) * yuv;
           final_color += vec4(rgb, 1) * _filter[i][j];
       }
   }
   return final_color;
}

void main()
{
    vec2 intXY = vec2(TexCoord.x * TexSize.x, TexCoord.y * TexSize.y);
    mat3 _smooth_fil1 =mat3(-1.0,-2.0,-1.0,
                           0.0, 0.0, 0.0,
                           1.0, 2.0, 1.0);
    mat3 _smooth_fil2 =mat3(-1.0, 0.0, 1.0,
                            -2.0, 0.0, 2.0,
                            -1.0, 0.0, 1.0);
    vec4 delColor1 = dip_filter(_smooth_fil1, ourTexture1, ourTexture2, ourTexture3, intXY, TexSize);
    vec4 delColor2 = dip_filter(_smooth_fil2, ourTexture1, ourTexture2, ourTexture3, intXY, TexSize);
    vec4 delColor = delColor2 + delColor2;
    float deltaGray = 0.3*delColor.x + 0.59*delColor.y + 0.11*delColor.z;
    if(deltaGray < 0.0) deltaGray = -1.0 * deltaGray;
    deltaGray = 1.0 - deltaGray;
    color = vec4(deltaGray,deltaGray,deltaGray,1.0);
}

/*sobel算子
 *mat3(-1.0,-2.0,-1.0,
        0.0, 0.0, 0.0,
        1.0, 2.0, 1.0)
  mat3(-1.0, 0.0, 1.0,
       -2.0, 0.0, 2.0,
       -1.0, 2.0, 1.0)*/
