#include "../include/vx_core/vx_model.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
#include <cmath>
#include <algorithm>

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
Mingw64 - Msys2

*/

#ifdef _WIN32
#define VERTX3D_BUILD_DLL
#endif

struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct vxModel
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    bool hasTexture;
    bool wireframe;
    float color[4];

    vxModel() : textureID(0), hasTexture(false), wireframe(false)
    {
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
    }
};

static bool loadOBJ(const char *filepath,
                    std::vector<Vertex> &out_vertices,
                    std::vector<unsigned int> &out_indices)
{

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        return false;
    }

    std::vector<float> temp_vertices;
    std::vector<float> temp_normals;
    std::vector<float> temp_uvs;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
        }
        else if (prefix == "vn")
        {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            temp_normals.push_back(nx);
            temp_normals.push_back(ny);
            temp_normals.push_back(nz);
        }
        else if (prefix == "vt")
        {
            float u, v;
            iss >> u >> v;
            temp_uvs.push_back(u);
            temp_uvs.push_back(v);
        }
        else if (prefix == "f")
        {
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;

            for (int i = 0; i < 3; i++)
            {
                std::string vertexStr = (i == 0) ? v1 : (i == 1) ? v2
                                                                 : v3;
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');

                std::istringstream viss(vertexStr);
                int vi, ti, ni;
                viss >> vi >> ti >> ni;

                vi--;
                ti--;
                ni--;

                Vertex vertex;

                if (vi >= 0 && vi * 3 + 2 < (int)temp_vertices.size())
                {
                    vertex.x = temp_vertices[vi * 3];
                    vertex.y = temp_vertices[vi * 3 + 1];
                    vertex.z = temp_vertices[vi * 3 + 2];
                }

                if (ni >= 0 && ni * 3 + 2 < (int)temp_normals.size())
                {
                    vertex.nx = temp_normals[ni * 3];
                    vertex.ny = temp_normals[ni * 3 + 1];
                    vertex.nz = temp_normals[ni * 3 + 2];
                }
                else
                {
                    vertex.nx = 0.0f;
                    vertex.ny = 1.0f;
                    vertex.nz = 0.0f;
                }

                if (ti >= 0 && ti * 2 + 1 < (int)temp_uvs.size())
                {
                    vertex.u = temp_uvs[ti * 2];
                    vertex.v = temp_uvs[ti * 2 + 1];
                }
                else
                {
                    vertex.u = 0.0f;
                    vertex.v = 0.0f;
                }

                out_vertices.push_back(vertex);
                out_indices.push_back(out_vertices.size() - 1);
            }
        }
    }

    file.close();
    return !out_vertices.empty();
}

static const char *modelVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

static const char *modelFragmentShader = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 color;
uniform bool useTexture;
uniform sampler2D textureSampler;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    
    vec4 objectColor;
    if (useTexture) {
        objectColor = texture(textureSampler, TexCoord);
    } else {
        objectColor = color;
    }
    
    
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
    FragColor = vec4(result, objectColor.a);
}
)";

static unsigned int modelShaderProgram = 0;

VX_API vxModel *vxLoadModel(const char *filepath)
{
    vxModel *model = new vxModel();

    if (!loadOBJ(filepath, model->vertices, model->indices))
    {
        delete model;
        return nullptr;
    }

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);

    glBindVertexArray(model->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 model->vertices.size() * sizeof(Vertex),
                 &model->vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 model->indices.size() * sizeof(unsigned int),
                 &model->indices[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, nx));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return model;
}

VX_API vxModel *vxLoadModelWithTexture(const char *objFile, const char *textureFile)
{
    vxModel *model = vxLoadModel(objFile);
    if (!model)
        return nullptr;

    unsigned char white[] = {255, 255, 255, 255};
    glGenTextures(1, &model->textureID);
    glBindTexture(GL_TEXTURE_2D, model->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    model->hasTexture = true;
    return model;
}

VX_API void vxDestroyModel(vxModel *model)
{
    if (!model)
        return;

    glDeleteVertexArrays(1, &model->VAO);
    glDeleteBuffers(1, &model->VBO);
    glDeleteBuffers(1, &model->EBO);

    if (model->textureID != 0)
    {
        glDeleteTextures(1, &model->textureID);
    }

    delete model;
}

VX_API void vxDrawModel(vxRenderContext *ctx, vxModel *model,
                        Vector3 position, float scale,
                        float rotationX, float rotationY, float rotationZ)
{
    if (!model || model->vertices.empty())
        return;

    if (modelShaderProgram == 0)
    {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &modelVertexShader, NULL);
        glCompileShader(vertexShader);

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &modelFragmentShader, NULL);
        glCompileShader(fragmentShader);

        modelShaderProgram = glCreateProgram();
        glAttachShader(modelShaderProgram, vertexShader);
        glAttachShader(modelShaderProgram, fragmentShader);
        glLinkProgram(modelShaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    glUseProgram(modelShaderProgram);

    if (model->wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    float modelMat[16] = {
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
        position.x, position.y, position.z, 1.0f};

    float viewMat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -5.0f, 1.0f};

    float aspect = 800.0f / 600.0f;
    float fov = 45.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov * 0.5f * 3.14159f / 180.0f);

    float projMat[16] = {0.0f};
    projMat[0] = f / aspect;
    projMat[5] = f;
    projMat[10] = (far + near) / (near - far);
    projMat[11] = -1.0f;
    projMat[14] = (2.0f * far * near) / (near - far);

    glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "model"), 1, GL_FALSE, modelMat);
    glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "view"), 1, GL_FALSE, viewMat);
    glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "projection"), 1, GL_FALSE, projMat);

    glUniform4f(glGetUniformLocation(modelShaderProgram, "color"),
                model->color[0], model->color[1], model->color[2], model->color[3]);

    glUniform1i(glGetUniformLocation(modelShaderProgram, "useTexture"), model->hasTexture ? 1 : 0);

    if (model->hasTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->textureID);
        glUniform1i(glGetUniformLocation(modelShaderProgram, "textureSampler"), 0);
    }

    glUniform3f(glGetUniformLocation(modelShaderProgram, "lightPos"), 5.0f, 5.0f, 5.0f);
    glUniform3f(glGetUniformLocation(modelShaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(modelShaderProgram, "viewPos"), 0.0f, 0.0f, 5.0f);

    glBindVertexArray(model->VAO);
    glDrawElements(GL_TRIANGLES, model->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

VX_API void vxModelSetColor(vxModel *model, float r, float g, float b, float a)
{
    if (model)
    {
        model->color[0] = r;
        model->color[1] = g;
        model->color[2] = b;
        model->color[3] = a;
    }
}

VX_API void vxModelSetWireframe(vxModel *model, bool wireframe)
{
    if (model)
    {
        model->wireframe = wireframe;
    }
}

VX_API int vxModelGetTriangleCount(vxModel *model)
{
    return model ? model->indices.size() / 3 : 0;
}