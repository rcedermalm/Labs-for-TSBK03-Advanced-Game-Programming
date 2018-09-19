#version 150

uniform sampler2D texUnit;
uniform sampler2D texUnit2;
in vec2 outTexCoord;
out vec4 out_Color;

void main(void)
{
  vec4 a  = texture(texUnit, outTexCoord);
  vec4 b  = texture(texUnit2, outTexCoord);
  out_Color = (a*0.7 + b*1.0);
}
