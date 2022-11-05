#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 fragpos;
out vec3 Normal;
//out vec3 ourColor;
//out vec2 TexCoord;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragpos = vec3(model * vec4(aPos, 1.0));
	Normal = aNormal;
	//ourColor = aColor;
	//TexCoord = aTexCoord;
};

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 fragpos;
in vec3 Normal;
//in vec3 ourColor;
//in vec2 TexCoord;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
uniform float ambientStrength;
void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragpos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
};