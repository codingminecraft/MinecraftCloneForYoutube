#include "renderer/Shader.h"

namespace MinecraftClone
{
    const char* vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec4 aColor;
layout (location = 1) in vec3 aPosition;

out vec4 fColor;

void main()
{
    fColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
})";

    const char* fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;

in vec4 fColor;

void main()
{
    FragColor = fColor;
})";

    void Shader::compile()
    {
        // Adapted from https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
        uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n '%s'", infoLog);
        }
        // fragment shader
        uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n '%s'", infoLog);
        }
        // link shaders
        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);
        // check for linking errors
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId, 512, NULL, infoLog);
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n '%s'", infoLog);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::bind()
    {
        glUseProgram(programId);
    }
}