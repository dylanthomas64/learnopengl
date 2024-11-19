#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform float x_offset;
uniform float y_offset;

uniform sampler2D texture2;


void main()
{
	// value 0.0 - 1.0
	//float intensity = texture(texture2, aTexCoord).r;
	// value -1.0 - 1.0
	//intensity = (intensity * 2) - 1;


	//vec3 newPos = aPos;

	//newPos.x += (intensity + sin(delta_time))/100;
	//newPos.y += (intensity + cos(delta_time))/100;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

