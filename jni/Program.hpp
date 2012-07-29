#ifndef XpgProgramHpp
#define XpgProgramHpp

#include "Shader.hpp"

namespace XPG
{
    template<size_t N>
    class Program
    {
        public:
            Program();
            ~Program();

            void AttachShader(GLuint shader);
            void BindAttribLocation(GLuint index, const GLchar* name);
            void Link();
            GLint GetUniformLocation(const GLchar* name);
            GLint GetAttribLocation(const GLchar* name);
            void Clear();

            inline void Use() const { glUseProgram(_handle); }

        private:
            GLuint _handle;
            GLuint _shaders[N];
            size_t _size;
            bool _isLinked;
    };

    template<size_t N>
    Program<N>::Program() : _handle(0), _size(0), _isLinked(false)
    {
    }

    template<size_t N>
    Program<N>::~Program()
    {
        Clear();
    }

    template<size_t N>
    void Program<N>::AttachShader(GLuint shader)
    {
        if (_isLinked)
        {
            // More shaders cannot be attached if the program is already linked.
        	LOGI("Program already linked!");
            return;
        }

        if (_size >= N)
        {
        	LOGI("No room for additional shader!");
            return;
        }

        if (!_size)
        {
            // If the shader container is empty, we have not yet created the
            // shader program. That needs to be done first!

            _handle = glCreateProgram();

            if (!_handle)
            {
            	LOGI("Failed to create program!");
                return;
            }
        }

        _shaders[_size++] = shader;

        glAttachShader(_handle, shader);
    }

    template<size_t N>
    void Program<N>::BindAttribLocation(GLuint index, const GLchar* name)
    {
        glBindAttribLocation(_handle, index, name);
    }

    template<size_t N>
    void Program<N>::Link()
    {
        if (_isLinked)
        {
            LOGI("Program already linked!");
            return;
        }

        if (_size < 2 || !_handle)
        {
            LOGI("Invalid program!");
            return;
        }

        glLinkProgram(_handle);

        GLint linked;
        glGetProgramiv(_handle, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            LOGI("Filed to link program!");
            return;
        }

        _isLinked = true;
        Use();
    }

    template<size_t N>
    GLint Program<N>::GetUniformLocation(const GLchar* name)
    {
        return glGetUniformLocation(_handle, name);
    }

    template<size_t N>
    GLint Program<N>::GetAttribLocation(const GLchar* name)
    {
    	return glGetAttribLocation(_handle, name);
    }

    template<size_t N>
    void Program<N>::Clear()
    {
        for (size_t i = 0; i < _size; ++i)
            glDetachShader(_handle, _shaders[i]);

        _size = 0;
        _isLinked = false;

        if (_handle)
        {
            glDeleteProgram(_handle);
            _handle = 0;
        }
    }
}

#endif
