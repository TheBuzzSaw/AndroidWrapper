#ifndef XpgShaderHpp
#define XpgShaderHpp

#include "Window.hpp"

namespace XPG
{
    class Shader
    {
        public:
            Shader(GLenum type);
            virtual ~Shader();

            inline GLuint Handle() const { return _handle; }
            void LoadFromBuffer(const char* source);

        private:
            Shader(const Shader& inShader)
            {
                // no copying
            }

            Shader& operator=(const Shader& inShader)
            {
                // no copying
                return *this;
            }

            GLuint _handle;
    };
}

#endif
