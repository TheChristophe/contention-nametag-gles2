#ifndef WRAPPERS_SHADER_HPP
#define WRAPPERS_SHADER_HPP

// Credits: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h

#include <GLES2/gl2.h>
#include <glm/glm.hpp> // glm types

#include <stdexcept>
#include <string_view>

namespace Wrappers {
    class ShaderError : public std::runtime_error {
        public:
        using std::runtime_error::runtime_error;
    };
    class Shader {
        public:
        // constructor generates the shader on the fly
        Shader(std::string_view vertexPath, std::string_view fragmentPath);

        // activate the shader
        void Use();

        // utility uniform functions
        bool Set(const char *name, const bool &value) const;
        bool Set(const char *name, const int &value) const;
        bool Set(const char *name, const float &value) const;
        bool Set(const char *name, const glm::vec2 &value) const;
        bool Set(const char *name, const glm::vec3 &value) const;
        bool Set(const char *name, const glm::vec4 &value) const;
        bool Set(const char *name, const glm::mat2 &mat) const;
        bool Set(const char *name, const glm::mat3 &mat) const;
        bool Set(const char *name, const glm::mat4 &mat) const;

        GLint GetAttribLocation(const char *name);

        private:
        // utility function for checking shader compilation/linking errors
        static void CheckCompileErrors(const GLuint &shader, const char *type);

        // opengl handle to shader object
        GLuint _id{};
    };
} // namespace Wrappers

#endif