#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

    // Calculate gradients here
    float offset = 1.0 / 256.0; // texture size, same in both directions
    float bt = texture(texUnit, outTexCoord).r - texture(texUnit, outTexCoord + vec2(0.0, offset)).r;
    float bs = texture(texUnit, outTexCoord).r - texture(texUnit, outTexCoord + vec2(offset, 0.0)).r;

    // 2a Bumpmapping in view coordinates
    vec3 normal = normalize(out_Normal - bs*Ps - bt*Pt);
    // Simplified lighting calculation.
    // A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4( dot(normal, light));


    // // 2b Bumpmapping in tex. coordinates
    // mat3 Mvt = transpose(mat3(Ps, Pt, out_Normal));
    // vec3 texlight = Mvt * light;
    // vec3 normal = normalize(vec3(bs, bt, 1.0));
    //
    // // Simplified lighting calculation.
    // // A full solution would include material, ambient, specular, light sources, multiply by texture.
    // out_Color = vec4( dot(normal, texlight));
}
