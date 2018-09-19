#version 150

uniform sampler2D texUnit;
in vec2 outTexCoord;
out vec4 out_Color;

void main(void)
{
  vec4 col = texture(texUnit, outTexCoord);
  out_Color.r = max(col.r - 1.0, 0.0);
  out_Color.g = max(col.g - 1.0, 0.0);
  out_Color.b = max(col.b - 1.0, 0.0);
}
