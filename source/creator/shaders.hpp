#pragma once

const char* vertexSource = R"glsl(

#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 Normal;
out vec3 Color;
out vec3 FragPos;
out vec3 LightPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    gl_PointSize = 5;
    Normal = mat3(transpose(inverse(model))) * normal;
    Color = color;
    FragPos = vec3(view * model * vec4(position, 1.0));
    LightPos = lightPos; vec3(view * vec4(lightPos, 1.0));
}

)glsl";


const char* fragmentSource = R"glsl(

#version 430 core

in vec3 Normal;
in vec3 Color;
in vec3 FragPos;
in vec3 LightPos;

//uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec4 objectColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse) * vec3(objectColor);
    FragColor = vec4(result, 1.0);
}

)glsl";

const char* gridVertexSource = R"glsl(

#version 430 core
uniform mat4 view;
uniform mat4 projection;
vec3 position = vec3(0, 0, 0);

// Grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);
// normal vertice projection
void main() {
    gl_Position = projection * view * vec4(gridPlane[gl_VertexID].xyz, 1.0);
}


)glsl";

const char* gridFragmentSource = R"glsl(

#version 430 core

const float tileSize = 0.2;
const float borderSize = 0.468;
const float lineBlur = 0.2;

vec2 aGrid(vec2 uv) {
  return vec2(mod(uv.x, 1.0), mod(uv.y * 2.0, 1.0));
}

vec2 bGrid(vec2 uv) {
  return vec2(mod(uv.x - 0.5, 1.0), mod((uv.y * 2.0) - 0.5, 1.0));
}

float los(vec2 pos) {
  vec2 abspos = abs(pos - vec2(0.5));
  return smoothstep(borderSize, borderSize + lineBlur, abspos.x + abspos.y);
}
out vec4 fragColor;
void main() {
  vec2 uv = vec2(1.0, 0.);//fragCoord;// / iResolution.xy;
  vec2 size = uv / tileSize;
  float alos = los(aGrid(size));
  float blos = los(bGrid(size));
  float color = min(alos, blos);
  color = pow(color, 1.0 / 2.2);
  fragColor = vec4(color);
}

)glsl";

/*
float4 HeatMapColor(float value, float minValue, float maxValue)
{
    #define HEATMAP_COLORS_COUNT 6
    float4 colors[HEATMAP_COLORS_COUNT] =
    {
        float4(0.32, 0.00, 0.32, 1.00),
        float4(0.00, 0.00, 1.00, 1.00),
        float4(0.00, 1.00, 0.00, 1.00),
        float4(1.00, 1.00, 0.00, 1.00),
        float4(1.00, 0.60, 0.00, 1.00),
        float4(1.00, 0.00, 0.00, 1.00),
    };
    float ratio=(HEATMAP_COLORS_COUNT-1.0)*saturate((value-minValue)/(maxValue-minValue));
    float indexMin=floor(ratio);
    float indexMax=min(indexMin+1,HEATMAP_COLORS_COUNT-1);
    return lerp(colors[indexMin], colors[indexMax], ratio-indexMin);
}
 * */