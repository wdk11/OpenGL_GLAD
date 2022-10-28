#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
uniform mat4 trans;
out vec3 ourColor;
out vec2 TexCoord;
void main()
{
	gl_Position = trans * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
};

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
};