#include "../include/vx_gui/vx_label.h"

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

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

struct Character
{
    unsigned int TextureID;
    int Width;
    int Height;
    int BearingX;
    int BearingY;
    unsigned int Advance;
};

struct vxLabel
{
    std::string text;
    std::string fontPath;
    int fontSize;
    Vector3 position;
    float color[3];
    std::map<char, Character> characters;
    unsigned int VAO, VBO;
    FT_Library ft;
    FT_Face face;
    bool fontLoaded;
};

VX_API vxLabel *vxNewLabel(const char *text, const char *fontPath, int fontSize,
                           Vector3 position, float r, float g, float b)
{
    vxLabel *label = new vxLabel();
    label->text = text ? text : "";
    label->fontPath = fontPath ? fontPath : "";
    label->fontSize = fontSize;
    label->position = position;
    label->color[0] = r;
    label->color[1] = g;
    label->color[2] = b;
    label->fontLoaded = false;

    if (FT_Init_FreeType(&label->ft))
    {
        delete label;
        return nullptr;
    }

    if (FT_New_Face(label->ft, fontPath, 0, &label->face))
    {
        FT_Done_FreeType(label->ft);
        delete label;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(label->face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {

        if (FT_Load_Char(label->face, c, FT_LOAD_RENDER))
        {
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            label->face->glyph->bitmap.width,
            label->face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            label->face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            label->face->glyph->bitmap.width,
            label->face->glyph->bitmap.rows,
            label->face->glyph->bitmap_left,
            label->face->glyph->bitmap_top,
            static_cast<unsigned int>(label->face->glyph->advance.x)};
        label->characters.insert(std::pair<char, Character>(c, character));
    }

    glGenVertexArrays(1, &label->VAO);
    glGenBuffers(1, &label->VBO);
    glBindVertexArray(label->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, label->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    label->fontLoaded = true;

    return label;
}

VX_API void vxDestroyLabel(vxLabel *label)
{
    if (!label)
        return;

    for (auto &pair : label->characters)
    {
        glDeleteTextures(1, &pair.second.TextureID);
    }

    if (label->VAO != 0)
        glDeleteVertexArrays(1, &label->VAO);
    if (label->VBO != 0)
        glDeleteBuffers(1, &label->VBO);

    if (label->face)
        FT_Done_Face(label->face);
    if (label->ft)
        FT_Done_FreeType(label->ft);

    delete label;
}

VX_API void vxLabelSetText(vxLabel *label, const char *text)
{
    if (label)
    {
        label->text = text ? text : "";
    }
}

VX_API void vxLabelSetPosition(vxLabel *label, Vector3 position)
{
    if (label)
    {
        label->position = position;
    }
}

VX_API void vxLabelSetColor(vxLabel *label, float r, float g, float b)
{
    if (label)
    {
        label->color[0] = r;
        label->color[1] = g;
        label->color[2] = b;
    }
}

VX_API void vxLabelSetFontSize(vxLabel *label, int fontSize)
{
    if (!label || !label->fontLoaded)
        return;

    label->fontSize = fontSize;
    FT_Set_Pixel_Sizes(label->face, 0, fontSize);

    label->characters.clear();

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(label->face, c, FT_LOAD_RENDER))
            continue;

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            label->face->glyph->bitmap.width,
            label->face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            label->face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            label->face->glyph->bitmap.width,
            label->face->glyph->bitmap.rows,
            label->face->glyph->bitmap_left,
            label->face->glyph->bitmap_top,
            static_cast<unsigned int>(label->face->glyph->advance.x)};
        label->characters.insert(std::pair<char, Character>(c, character));
    }
}

static const char *textVertexShader = R"(
#version 330 core
layout (location = 0) in vec4 vertex; 
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

static const char *textFragmentShader = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

static unsigned int textShaderProgram = 0;
static int textProjectionLoc = -1;
static int textTextLoc = -1;
static int textColorLoc = -1;

static void initTextShader()
{
    if (textShaderProgram != 0)
        return;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &textVertexShader, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &textFragmentShader, NULL);
    glCompileShader(fragmentShader);

    textShaderProgram = glCreateProgram();
    glAttachShader(textShaderProgram, vertexShader);
    glAttachShader(textShaderProgram, fragmentShader);
    glLinkProgram(textShaderProgram);

    textProjectionLoc = glGetUniformLocation(textShaderProgram, "projection");
    textTextLoc = glGetUniformLocation(textShaderProgram, "text");
    textColorLoc = glGetUniformLocation(textShaderProgram, "textColor");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

VX_API void vxDrawLabel(vxRenderContext *ctx, vxLabel *label)
{
    if (!label || !label->fontLoaded || label->text.empty())
        return;

    initTextShader();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(textShaderProgram);
    glUniform3f(textColorLoc, label->color[0], label->color[1], label->color[2]);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textTextLoc, 0);

    int width = 800;
    int height = 600;

    float projection[16] = {
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f};

    glUniformMatrix4fv(textProjectionLoc, 1, GL_FALSE, projection);

    float x = label->position.x;
    float y = label->position.y;

    glBindVertexArray(label->VAO);

    std::string::const_iterator c;
    for (c = label->text.begin(); c != label->text.end(); c++)
    {
        Character ch = label->characters[*c];

        float xpos = x + ch.BearingX;
        float ypos = height - y - (ch.Height - ch.BearingY);

        float w = ch.Width;
        float h = ch.Height;

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, label->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}