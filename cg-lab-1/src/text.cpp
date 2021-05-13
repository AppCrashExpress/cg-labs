#include <glad/glad.h>
#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>
#include <string>

#include "text.h"
#include "line.h"

TextRenderer::TextRenderer(const std::string& loadedSymbols,
        const GLint SCR_WIDTH, const GLint SCR_HEIGHT) :
        srcWidth(SCR_WIDTH), srcHeight(SCR_HEIGHT) { 
    init(loadedSymbols);
}

TextRenderer::~TextRenderer() {
    for (const auto& e : charMap) {
        glDeleteTextures(1, &e.second.textureId);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void TextRenderer::renderFloat(float number, GLfloat x, GLfloat y,
        GLfloat scale, Coords offset) {
    std::string converted = std::to_string(number);
    std::string numStr = converted.substr(0, converted.find('.') + 3);

    render(numStr, x, y, scale, offset);
}

void TextRenderer::render(const std::string& text, GLfloat x, GLfloat y,
        GLfloat scale, Coords offset) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    for (const auto& ch : text) {
        const Character graphCh = charMap[ch];

        const GLfloat xpos = x + graphCh.bearing.x * scale;
        const GLfloat ypos = y - (graphCh.size.y - graphCh.bearing.y) * scale;

        const GLfloat w = graphCh.size.x * scale;
        const GLfloat h = graphCh.size.y * scale;

        const GLfloat vertices[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos,     ypos,     0.0f, 1.0f},
            {xpos + w, ypos,     1.0f, 1.0f},

            {xpos,     ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, graphCh.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (graphCh.advance >> 6) * scale / srcWidth;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::init(const std::string& loadedSymbols) {
    initBuffers();

    FT_Library ftLib;
    if (FT_Init_FreeType(&ftLib) != 0) {
        std::cerr << "ERROR: Failed to load FreeType library" << std::endl;
        return;
    }

    FT_Face ftFace;
    if (FT_New_Face(ftLib, "assets/fonts/arial.ttf", 0, &ftFace) != 0) {
        std::cerr << "ERROR: Failed to load font face" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(ftFace, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 


    for (const char& ch : loadedSymbols) {
        loadChar(ch, ftFace);
    }

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
}

void TextRenderer::initBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::loadChar(const char ch, const FT_Face ftFace) {
    if (FT_Load_Char(ftFace, ch, FT_LOAD_RENDER)) {
        std::cerr << "ERROR: Failed to load character glyph: " << ch << std::endl;
    }

    Character graphicCh = {
        0,
        static_cast<float>(ftFace->glyph->advance.x),
        {
            static_cast<float>(ftFace->glyph->bitmap.width) / srcWidth,
            static_cast<float>(ftFace->glyph->bitmap.rows) / srcHeight
        },
        {
            static_cast<float>(ftFace->glyph->bitmap_left) / srcWidth,
            static_cast<float>(ftFace->glyph->bitmap_top) / srcHeight
        }
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        ftFace->glyph->bitmap.width,
        ftFace->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        ftFace->glyph->bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    graphicCh.textureId = texture;
    charMap.insert(std::make_pair(ch, graphicCh));
}
