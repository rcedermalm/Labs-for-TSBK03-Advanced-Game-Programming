#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;

uniform mat4 trans_bone0;
uniform mat4 trans_bone1;
uniform mat4 rot_bone0;
uniform mat4 rot_bone1;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{
	// TASK 1 & 2: Activate this
	//gl_Position = matrix * vec4(in_Position, 1.0);

	// TASK 3 Skinning with two bones on GPU
	mat4 Mbone0 = trans_bone0;
	mat4 Mbone1 = trans_bone1;

	mat4 Mboneprim0 = Mbone0 * rot_bone0;
	mat4 Mboneprim1 = Mbone1 * rot_bone1;

	mat4 M0 = Mbone0 * inverse(Mbone0);
	mat4 M1 = Mboneprim0 * Mboneprim1 * inverse(Mbone1) * inverse(Mbone0);

	float w0 = in_TexCoord.x;
	float w1 = in_TexCoord.y;
	vec4 new_pos = w0 * M0 * vec4(in_Position, 1.0) + w1 * M1 * vec4(in_Position, 1.0);

	// transformera resultatet med ModelView- och Projection-matriserna
	gl_Position = matrix * new_pos;

	///////////////////////////////////////////////////////////////

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}
