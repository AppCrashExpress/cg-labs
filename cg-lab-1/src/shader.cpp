#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include "shader.h"
#define NULL 0

Shader::Shader(const char* vsFilePath, const char* fsFilePath)
{
    const char* vShaderSrc = LoadFromFile(vsFilePath);
    const char* fShaderSrc = LoadFromFile(fsFilePath);

    if (vShaderSrc && fShaderSrc)
    {
        CreateShader(vShaderSrc, fShaderSrc);
    }

    if (vShaderSrc) { delete vShaderSrc; }
    if (fShaderSrc) { delete fShaderSrc; }
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}

void Shader::CreateShader(const char* vShaderSrc, const char* fShaderSrc)
{
    unsigned int vShader, fShader;
    int success;
    char infoLog[512];

    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glCompileShader(vShader);

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cerr << "ERROR::VERTEX_SHADER::COMPILE_FAILED\n"
                  << infoLog << std::endl;
    }

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cerr << "ERROR::FRAGMENT_SHADER::COMPILE_FAILED\n"
                  << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

unsigned int Shader::Get()
{
    return shaderProgram;
}

char* Shader::LoadFromFile(const char* shaderPos)
{
    std::ifstream file(shaderPos);

    if (!file.is_open())
    {
        std::cerr << "ERROR::File " << shaderPos
                  << " failed to load" << std::endl;
        return nullptr;
    }

    std::string str((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();

    const char* str_c = str.c_str();
    char* shader = new char[str.size() + 1];
    for (size_t i = 0; i < str.size(); ++i)
    {
        shader[i] = str_c[i];
    }
    shader[str.size()] = '\0';

    return shader;
}