#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_coordinate;
in vec3 frag_color;

layout(location = 0) out vec4 out_color;
uniform vec3 object_color;

void main()
{
	// TODO: write pixel out color
	//out_color = vec4(0.251, 0.878, 0.816, 1);
	out_color = vec4(object_color, 1);
}