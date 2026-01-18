
#include "../include/vx_gui/vx_frame.h"
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cmath>

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

#ifdef _WIN32
#define VERTX3D_BUILD_DLL
#endif

struct vxFrame
{
    float width, height;
    Vector3 position;
    float color[4];
    bool visible;
    bool is2D;
    float borderThickness;
    float borderColor[4];
    float cornerRadius;

    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
};

static const char *frameVertexShader = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 projection;
uniform mat4 model;

void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
}
)";

static const char *frameFragmentShader = R"(
#version 330 core
out vec4 FragColor;

in vec2 FragPos;

uniform vec4 color;
uniform vec2 size;
uniform float cornerRadius;
uniform float borderThickness;
uniform vec4 borderColor;
uniform bool is2D;

void main() {
    vec2 halfSize = size * 0.5;
    vec2 centerAdjusted = abs(FragPos) - (halfSize - vec2(cornerRadius));
    
    
    float distance = length(max(centerAdjusted, 0.0)) + min(max(centerAdjusted.x, centerAdjusted.y), 0.0);
    
    
    float smoothness = 1.0;
    
    
    if (borderThickness > 0.0) {
        float borderInner = cornerRadius - borderThickness;
        vec2 borderCenterAdjusted = abs(FragPos) - (halfSize - vec2(borderInner));
        float borderDistance = length(max(borderCenterAdjusted, 0.0)) + min(max(borderCenterAdjusted.x, borderCenterAdjusted.y), 0.0);
        
        if (distance <= cornerRadius + smoothness) {
            if (borderDistance <= borderInner + smoothness) {
                
                float alpha = smoothstep(cornerRadius, cornerRadius - smoothness, distance);
                FragColor = mix(vec4(0.0), color, alpha);
            } else {
                
                float borderAlpha = smoothstep(borderInner, borderInner + smoothness, borderDistance);
                float edgeAlpha = smoothstep(cornerRadius, cornerRadius - smoothness, distance);
                float alpha = borderAlpha * edgeAlpha;
                FragColor = mix(color, borderColor, alpha);
            }
        } else {
            discard;
        }
    } else {
        
        if (distance <= cornerRadius + smoothness) {
            float alpha = smoothstep(cornerRadius, cornerRadius - smoothness, distance);
            FragColor = mix(vec4(0.0), color, alpha);
        } else {
            discard;
        }
    }
}
)";

static unsigned int compileShaderProgram(const char *vertexSource, const char *fragmentSource)
{

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

VX_API vxFrame *vxNewFrame(float width, float height, Vector3 position, float r, float g, float b, float a)
{
    vxFrame *frame = new vxFrame();
    frame->width = width;
    frame->height = height;
    frame->position = position;
    frame->color[0] = r;
    frame->color[1] = g;
    frame->color[2] = b;
    frame->color[3] = a;
    frame->visible = true;
    frame->is2D = true;
    frame->borderThickness = 0.0f;
    frame->borderColor[0] = 0.0f;
    frame->borderColor[1] = 0.0f;
    frame->borderColor[2] = 0.0f;
    frame->borderColor[3] = 1.0f;
    frame->cornerRadius = 0.0f;

    frame->shaderProgram = compileShaderProgram(frameVertexShader, frameFragmentShader);

    float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f};

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    glGenVertexArrays(1, &frame->VAO);
    glGenBuffers(1, &frame->VBO);
    glGenBuffers(1, &frame->EBO);

    glBindVertexArray(frame->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, frame->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frame->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return frame;
}

VX_API vxFrame *vxCreateFrame(float width, float height, float x, float y, float z,
                              float r, float g, float b, float a)
{
    return vxNewFrame(width, height, Vector3{x, y, z}, r, g, b, a);
}

VX_API void vxDestroyFrame(vxFrame *frame)
{
    if (!frame)
        return;

    glDeleteVertexArrays(1, &frame->VAO);
    glDeleteBuffers(1, &frame->VBO);
    glDeleteBuffers(1, &frame->EBO);
    glDeleteProgram(frame->shaderProgram);

    delete frame;
}

VX_API void vxFrameSetPosition(vxFrame *frame, Vector3 position)
{
    if (frame)
    {
        frame->position = position;
    }
}

VX_API void vxFrameSetPositionF(vxFrame *frame, float x, float y, float z)
{
    if (frame)
    {
        frame->position = Vector3{x, y, z};
    }
}

VX_API void vxFrameSetSize(vxFrame *frame, float width, float height)
{
    if (frame)
    {
        frame->width = width;
        frame->height = height;
    }
}

VX_API void vxFrameSetColor(vxFrame *frame, float r, float g, float b, float a)
{
    if (frame)
    {
        frame->color[0] = r;
        frame->color[1] = g;
        frame->color[2] = b;
        frame->color[3] = a;
    }
}

VX_API void vxFrameSetVisible(vxFrame *frame, bool visible)
{
    if (frame)
    {
        frame->visible = visible;
    }
}

VX_API void vxFrameSet2D(vxFrame *frame, bool is2D)
{
    if (frame)
    {
        frame->is2D = is2D;
    }
}

VX_API void vxFrameSetBorder(vxFrame *frame, float thickness, float r, float g, float b, float a)
{
    if (frame)
    {
        frame->borderThickness = thickness;
        frame->borderColor[0] = r;
        frame->borderColor[1] = g;
        frame->borderColor[2] = b;
        frame->borderColor[3] = a;
    }
}

VX_API void vxFrameSetCornerRadius(vxFrame *frame, float radius)
{
    if (frame)
    {
        frame->cornerRadius = radius;
    }
}

VX_API void vxDrawFrame(vxRenderContext *ctx, vxFrame *frame)
{
    if (!frame || !frame->visible || frame->color[3] <= 0.001f)
    {
        return;
    }

    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (frame->is2D)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
    }

    glUseProgram(frame->shaderProgram);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float screenWidth = (float)viewport[2];
    float screenHeight = (float)viewport[3];

    if (frame->is2D)
    {

        float projection[16] = {
            2.0f / screenWidth, 0.0f, 0.0f, 0.0f,
            0.0f, -2.0f / screenHeight, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f};

        float model[16] = {
            frame->width, 0.0f, 0.0f, 0.0f,
            0.0f, frame->height, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            frame->position.x, frame->position.y, 0.0f, 1.0f};

        glUniformMatrix4fv(glGetUniformLocation(frame->shaderProgram, "projection"), 1, GL_FALSE, projection);
        glUniformMatrix4fv(glGetUniformLocation(frame->shaderProgram, "model"), 1, GL_FALSE, model);
    }
    else
    {

        float fov = 45.0f;
        float aspect = screenWidth / screenHeight;
        float near = 0.1f;
        float far = 100.0f;

        float proj[16] = {0.0f};
        float f = 1.0f / tan(fov * 0.5f * 3.14159f / 180.0f);

        proj[0] = f / aspect;
        proj[5] = f;
        proj[10] = (far + near) / (near - far);
        proj[11] = -1.0f;
        proj[14] = (2.0f * far * near) / (near - far);

        float model[16] = {
            frame->width, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, frame->height, 0.0f,
            frame->position.x, frame->position.y, frame->position.z, 1.0f};

        glUniformMatrix4fv(glGetUniformLocation(frame->shaderProgram, "projection"), 1, GL_FALSE, proj);
        glUniformMatrix4fv(glGetUniformLocation(frame->shaderProgram, "model"), 1, GL_FALSE, model);
    }

    glUniform4f(glGetUniformLocation(frame->shaderProgram, "color"),
                frame->color[0], frame->color[1], frame->color[2], frame->color[3]);
    glUniform2f(glGetUniformLocation(frame->shaderProgram, "size"), frame->width, frame->height);
    glUniform1f(glGetUniformLocation(frame->shaderProgram, "cornerRadius"), frame->cornerRadius);
    glUniform1f(glGetUniformLocation(frame->shaderProgram, "borderThickness"), frame->borderThickness);
    glUniform4f(glGetUniformLocation(frame->shaderProgram, "borderColor"),
                frame->borderColor[0], frame->borderColor[1],
                frame->borderColor[2], frame->borderColor[3]);
    glUniform1i(glGetUniformLocation(frame->shaderProgram, "is2D"), frame->is2D ? 1 : 0);

    glBindVertexArray(frame->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (!blendEnabled)
        glDisable(GL_BLEND);
    if (depthTest && !frame->is2D)
        glEnable(GL_DEPTH_TEST);
}