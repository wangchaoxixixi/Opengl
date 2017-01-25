#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;
uniform vec2 TexSize;

void main()
{
   vec2 tex =TexCoord;
   vec2 upLeftUV = vec2(tex.x-1.0/TexSize.x,tex.y-1.0/TexSize.y);
   vec4 curColor = texture2D(ourTexture,TexCoord);
   vec4 upLeftColor = texture2D(ourTexture,upLeftUV);
   vec4 delColor = curColor - upLeftColor;
   float h = 0.3*delColor.x + 0.59*delColor.y + 0.11*delColor.z;
   vec4 bkColor = vec4(0.5, 0.5, 0.5, 1.0);
   color = vec4(h,h,h,0.0) +bkColor;
}
