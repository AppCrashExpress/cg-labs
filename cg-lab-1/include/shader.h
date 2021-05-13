#pragma once
#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader() { shaderProgram = 0; };
    Shader(const char*, const char*);
    ~Shader();

    void         CreateShader(const char*, const char*);
    unsigned int Get();
    
private:
    char* LoadFromFile(const char*);

    unsigned int shaderProgram;
};

#endif