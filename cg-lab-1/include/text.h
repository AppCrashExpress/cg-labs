#ifndef GRAPH_TEXT_H
#define GRAPH_TEXT_H

#include "ft2build.h"
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <string>
#include <map>

#include "line.h"

class TextRenderer {
public:
    TextRenderer(const std::string& loadedSymbols, 
        const GLint SCR_WIDTH, const GLint SCR_HEIGHT);
    ~TextRenderer();

    void renderFloat(float number, GLfloat x, GLfloat y, 
        GLfloat scale, Coords offset);
    void render(const std::string& text, GLfloat x, GLfloat y,
        GLfloat scale, Coords offset);

private:
    struct Character {
        GLuint       textureId;
        unsigned int advance;

        struct {
            GLfloat x, y;
        } size, bearing;
    };

    std::map<char, Character> charMap;
    GLuint vao, vbo;

    const GLint srcWidth, srcHeight;
    
    void init(const std::string& loadedSymbols);
    void initBuffers();
    void loadChar(const char ch, const FT_Face ftFace);
};

#endif