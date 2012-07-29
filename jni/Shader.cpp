#include "Shader.hpp"

namespace XPG
{
    Shader::Shader(GLenum type)
    {
        _handle = glCreateShader(type);
    }

    Shader::~Shader()
    {
        //if (_handle) glDeleteShader(_handle);
    }

    void Shader::LoadFromBuffer(const char* source)
    {
        glShaderSource(_handle, 1, const_cast<const GLchar**>(&source), 0);
        glCompileShader(_handle);

        GLint compiled;
        glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            GLchar log[2048];
            GLsizei length;
            glGetShaderInfoLog(_handle, 2048, &length, log);

            LOGI("shader compiler errors -- %s", log);
        }
    }
}
