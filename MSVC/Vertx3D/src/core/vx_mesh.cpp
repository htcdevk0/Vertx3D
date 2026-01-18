#include "../../include/vx_core/vx_mesh.h"

#ifdef _WIN32
#define VERTX3D_BUILD_DLL
#endif

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
MSVC - Visual Studio 2022

*/

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cfloat>
#include <cmath>

static int currentWindowWidth = 800;
static int currentWindowHeight = 600;

struct vxMesh
{
    unsigned int VAO, VBO;
    int vertexCount;
};

static float cubeVertices[] = {

    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

static std::vector<float> generateSphereVertices(int sectors, int stacks, float radius)
{
    std::vector<float> vertices;
    const float PI = 3.14159265358979323846f;

    std::vector<float> points;
    for (int i = 0; i <= stacks; ++i)
    {
        float v = (float)i / stacks;
        float phi = v * PI;

        for (int j = 0; j <= sectors; ++j)
        {
            float u = (float)j / sectors;
            float theta = u * 2.0f * PI;

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            points.push_back(x * radius);
            points.push_back(y * radius);
            points.push_back(z * radius);

            points.push_back(0.5f + 0.5f * x);
            points.push_back(0.5f + 0.5f * y);
            points.push_back(0.5f + 0.5f * z);

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int first = (i * (sectors + 1)) + j;
            int second = first + sectors + 1;

            for (int k = 0; k < 9; k++)
                vertices.push_back(points[first * 9 + k]);
            for (int k = 0; k < 9; k++)
                vertices.push_back(points[second * 9 + k]);
            for (int k = 0; k < 9; k++)
                vertices.push_back(points[(first + 1) * 9 + k]);

            for (int k = 0; k < 9; k++)
                vertices.push_back(points[second * 9 + k]);
            for (int k = 0; k < 9; k++)
                vertices.push_back(points[(second + 1) * 9 + k]);
            for (int k = 0; k < 9; k++)
                vertices.push_back(points[(first + 1) * 9 + k]);
        }
    }

    return vertices;
}

static float planeVertices[] = {

    -0.5f, 0.0f, -0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, -0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, 0.8f, 0.8f, 0.8f, 0.0f, 1.0f, 0.0f};

static float triangleVertices[] = {

    0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

static float quadVertices[] = {

    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 overrideColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    if (overrideColor.x >= 0.0) {
        ourColor = overrideColor;
    } else {
        ourColor = aColor;
    }
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
    
    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0);
}
)";

static unsigned int shaderProgram = 0;
static int modelLoc = -1;
static int viewLoc = -1;
static int projLoc = -1;
static int colorLoc = -1;
static int lightPosLoc = -1;
static int viewPosLoc = -1;

VX_API void vxUpdateWindowSize(int width, int height)
{
    currentWindowWidth = width;
    currentWindowHeight = height;
}

struct LightConfig
{
    int type;
    Vector3 color;
    float intensity;
    Vector3 position;
    Vector3 direction;
    float cutoff;
    float outerCutoff;
};

static LightConfig currentLight = {
    VX_LIGHT_POINT,
    {1.0f, 1.0f, 1.0f},
    1.0f,
    {5.0f, 5.0f, 5.0f},
    {0.0f, -1.0f, 0.0f},
    cos(12.5f * 3.14159f / 180.0f),
    cos(17.5f * 3.14159f / 180.0f)};

static float ambientLight[3] = {0.1f, 0.1f, 0.1f};

static const char *lightingVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 overrideColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    if (overrideColor.x >= 0.0) {
        ourColor = overrideColor;
    } else {
        ourColor = aColor;
    }
}
)";

static const char *lightingFragmentShader = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;


uniform int lightType;          
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightPos;          
uniform vec3 lightDir;          
uniform float lightCutoff;
uniform float lightOuterCutoff;
uniform vec3 ambientColor;
uniform vec3 viewPos;

void main() {
    
    vec3 norm = normalize(Normal);
    vec3 lightDirNormalized;
    
    
    if (lightType == 0) { 
        lightDirNormalized = normalize(-lightDir);
    } else { 
        lightDirNormalized = normalize(lightPos - FragPos);
    }
    
    
    vec3 ambient = ambientColor * ourColor;
    
    
    float diff = max(dot(norm, lightDirNormalized), 0.0);
    vec3 diffuse = diff * lightColor * ourColor * lightIntensity;
    
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirNormalized, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = 0.5 * spec * lightColor * lightIntensity;
    
    
    float attenuation = 1.0;
    if (lightType == 1 || lightType == 2) {
        float distance = length(lightPos - FragPos);
        attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    }
    
    
    float theta = dot(lightDirNormalized, normalize(-lightDir));
    float epsilon = lightCutoff - lightOuterCutoff;
    float spotIntensity = 1.0;
    
    if (lightType == 2) { 
        if (theta > lightCutoff) {
            spotIntensity = 1.0;
        } else if (theta > lightOuterCutoff) {
            spotIntensity = (theta - lightOuterCutoff) / epsilon;
        } else {
            spotIntensity = 0.0;
        }
    }
    
    
    vec3 result = ambient + (diffuse + specular) * attenuation * spotIntensity;
    
    
    result = min(result, vec3(1.0));
    
    FragColor = vec4(result, 1.0);
}
)";

VX_API void vxSetupLighting(int type, float r, float g, float b, float intensity, float x, float y, float z)
{
    currentLight.type = type;
    currentLight.color = {r, g, b};
    currentLight.intensity = intensity;

    if (type == VX_LIGHT_DIRECTIONAL)
    {
        currentLight.direction = {x, y, z};
    }
    else
    {
        currentLight.position = {x, y, z};
        if (type == VX_LIGHT_SPOT)
        {
            currentLight.direction = {0.0f, -1.0f, 0.0f};
        }
    }
}

VX_API void vxSetAmbientLight(float r, float g, float b)
{
    ambientLight[0] = r;
    ambientLight[1] = g;
    ambientLight[2] = b;
}

static void initShaders()
{
    if (shaderProgram != 0)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &lightingVertexShader, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &lightingFragmentShader, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    colorLoc = glGetUniformLocation(shaderProgram, "overrideColor");

    int lightTypeLoc = glGetUniformLocation(shaderProgram, "lightType");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    int lightIntensityLoc = glGetUniformLocation(shaderProgram, "lightIntensity");
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    int lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    int lightCutoffLoc = glGetUniformLocation(shaderProgram, "lightCutoff");
    int lightOuterCutoffLoc = glGetUniformLocation(shaderProgram, "lightOuterCutoff");
    int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
    viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glUseProgram(shaderProgram);
    if (lightTypeLoc != -1)
        glUniform1i(lightTypeLoc, currentLight.type);
    if (lightColorLoc != -1)
        glUniform3f(lightColorLoc, currentLight.color.x, currentLight.color.y, currentLight.color.z);
    if (lightIntensityLoc != -1)
        glUniform1f(lightIntensityLoc, currentLight.intensity);
    if (lightPosLoc != -1)
        glUniform3f(lightPosLoc, currentLight.position.x, currentLight.position.y, currentLight.position.z);
    if (lightDirLoc != -1)
        glUniform3f(lightDirLoc, currentLight.direction.x, currentLight.direction.y, currentLight.direction.z);
    if (lightCutoffLoc != -1)
        glUniform1f(lightCutoffLoc, currentLight.cutoff);
    if (lightOuterCutoffLoc != -1)
        glUniform1f(lightOuterCutoffLoc, currentLight.outerCutoff);
    if (ambientColorLoc != -1)
        glUniform3f(ambientColorLoc, ambientLight[0], ambientLight[1], ambientLight[2]);
    if (viewPosLoc != -1)
        glUniform3f(viewPosLoc, 0.0f, 0.0f, 8.0f);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

VX_API vxMesh *vxNewMesh(int meshType)
{
    initShaders();

    vxMesh *mesh = new vxMesh();

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    switch (meshType)
    {
    case VX_MESH_CUBE:
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        mesh->vertexCount = 36;
        break;

    case VX_MESH_SPHERE:
    {

        std::vector<float> sphereVerts = generateSphereVertices(36, 18, 0.5f);
        glBufferData(GL_ARRAY_BUFFER, sphereVerts.size() * sizeof(float), sphereVerts.data(), GL_STATIC_DRAW);

        mesh->vertexCount = sphereVerts.size() / 9;
        break;
    }

    case VX_MESH_PLANE:
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        mesh->vertexCount = 6;
        break;

    case VX_MESH_TRIANGLE:
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        mesh->vertexCount = 3;
        break;

    case VX_MESH_QUAD:
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        mesh->vertexCount = 6;
        break;

    default:
        std::cerr << "Mesh type " << meshType << " not supported!" << std::endl;
        mesh->vertexCount = 0;
        return mesh;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return mesh;
}

VX_API void vxDrawMesh(vxRenderContext *ctx, vxMesh *mesh, float r, float g, float b, float a, Vector3 position, float size, float rotation, float tilt)
{
    if (!mesh || mesh->vertexCount == 0)
        return;

    if (shaderProgram == 0)
    {
        initShaders();
    }

    glUseProgram(shaderProgram);

    int lightTypeLoc = glGetUniformLocation(shaderProgram, "lightType");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    int lightIntensityLoc = glGetUniformLocation(shaderProgram, "lightIntensity");
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    int lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
    int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    if (lightTypeLoc != -1)
        glUniform1i(lightTypeLoc, currentLight.type);
    if (lightColorLoc != -1)
        glUniform3f(lightColorLoc, currentLight.color.x, currentLight.color.y, currentLight.color.z);
    if (lightIntensityLoc != -1)
        glUniform1f(lightIntensityLoc, currentLight.intensity);
    if (lightPosLoc != -1)
        glUniform3f(lightPosLoc, currentLight.position.x, currentLight.position.y, currentLight.position.z);
    if (lightDirLoc != -1)
        glUniform3f(lightDirLoc, currentLight.direction.x, currentLight.direction.y, currentLight.direction.z);
    if (ambientColorLoc != -1)
        glUniform3f(ambientColorLoc, ambientLight[0], ambientLight[1], ambientLight[2]);
    if (viewPosLoc != -1)
        glUniform3f(viewPosLoc, 0.0f, 0.0f, 8.0f);

    float model[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    model[0] = size;
    model[5] = size;
    model[10] = size;

    if (rotation != 0.0f)
    {
        float adjustedRotation = rotation * 0.02f;
        float cosY = cos(adjustedRotation);
        float sinY = sin(adjustedRotation);

        float rotY[16] = {
            cosY, 0.0f, sinY, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sinY, 0.0f, cosY, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        float temp[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    temp[i * 4 + j] += model[i * 4 + k] * rotY[k * 4 + j];
                }
            }
        }
        memcpy(model, temp, sizeof(float) * 16);
    }

    if (tilt != 0.0f)
    {
        float adjustedTilt = tilt * 0.02f;
        float cosX = cos(adjustedTilt);
        float sinX = sin(adjustedTilt);

        float rotX[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosX, -sinX, 0.0f,
            0.0f, sinX, cosX, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        float temp[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    temp[i * 4 + j] += model[i * 4 + k] * rotX[k * 4 + j];
                }
            }
        }
        memcpy(model, temp, sizeof(float) * 16);
    }
    if (position.z <= 0.0f)
        position.z = 1.0f;

    model[12] = position.x;
    model[13] = position.y;
    model[14] = position.z;

    float view[16];
    float proj[16];

    if (camera_active && !std::isnan(camera_view_matrix[0]) && camera_view_matrix[0] != 0)
    {

        memcpy(view, camera_view_matrix, sizeof(float) * 16);
        memcpy(proj, camera_proj_matrix, sizeof(float) * 16);
    }
    else
    {

        float view_fixed[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, -10.0f, 1.0f};
        memcpy(view, view_fixed, sizeof(float) * 16);

        float fov = 60.0f;
        float aspect = (float)currentWindowWidth / (float)currentWindowHeight;
        if (aspect <= 0.0f)
            aspect = 1.0f;
        float near = 0.1f;
        float far = 1000.0f;

        memset(proj, 0, sizeof(float) * 16);
        float f = 1.0f / tan(fov * 0.5f * 3.14159f / 180.0f);

        proj[0] = f / aspect;
        proj[5] = f;
        proj[10] = (far + near) / (near - far);
        proj[11] = -1.0f;
        proj[14] = (2.0f * far * near) / (near - far);
    }

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);

    if (r >= 0.0f)
    {
        float color[3] = {r, g, b};
        glUniform3fv(colorLoc, 1, color);
    }
    else
    {
        float color[3] = {-1.0f, 0.0f, 0.0f};
        glUniform3fv(colorLoc, 1, color);
    }

    float lightPos[3] = {5.0f, 5.0f, 5.0f};
    float viewPos[3] = {0.0f, 0.0f, 8.0f};
    glUniform3fv(lightPosLoc, 1, lightPos);
    glUniform3fv(viewPosLoc, 1, viewPos);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
}

VX_API void vxDestroyMesh(vxMesh *mesh)
{
    if (!mesh)
        return;

    if (mesh->VAO != 0)
    {
        glDeleteVertexArrays(1, &mesh->VAO);
    }

    if (mesh->VBO != 0)
    {
        glDeleteBuffers(1, &mesh->VBO);
    }

    delete mesh;
}

VX_API vxMesh *vxCreateMeshFromVertices(vxVertex *vertices, int vertexCount)
{
    if (!vertices || vertexCount <= 0)
        return nullptr;

    initShaders();

    vxMesh *mesh = new vxMesh();

    std::vector<float> vertexData;
    vertexData.reserve(vertexCount * 9);

    for (int i = 0; i < vertexCount; i++)
    {

        vertexData.push_back(vertices[i].x);
        vertexData.push_back(vertices[i].y);
        vertexData.push_back(vertices[i].z);

        vertexData.push_back(vertices[i].r);
        vertexData.push_back(vertices[i].g);
        vertexData.push_back(vertices[i].b);

        vertexData.push_back(vertices[i].nx);
        vertexData.push_back(vertices[i].ny);
        vertexData.push_back(vertices[i].nz);
    }

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    mesh->vertexCount = vertexCount;

    return mesh;
}

VX_API vxMesh *vxCreateMeshFromArray(float *vertices, int vertexCount)
{
    if (!vertices || vertexCount <= 0)
        return nullptr;

    initShaders();

    vxMesh *mesh = new vxMesh();

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    mesh->vertexCount = vertexCount;

    return mesh;
}