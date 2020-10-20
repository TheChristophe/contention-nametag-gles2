#include "shader.hpp"

#include "util/io.hpp" // util::StringFromFile

#include <iostream>

#ifdef DEBUGGING
static constexpr bool debugging{ true };
#else
static constexpr bool debugging{ false };
#endif

namespace Wrappers {
    Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath)
    {
        auto vertexOpt{ util::StringFromFile(vertexPath) };
        if (vertexOpt.has_value() == false) {
            throw ShaderError(std::string("failed to load vertex shader ") + std::string(vertexPath));
        }
        auto vertexShader{ vertexOpt.value() };

        // compile vertex shader
        GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) };
        const GLchar *vertexCode{ vertexShader.c_str() };
        glShaderSource(vertex, 1, &vertexCode, nullptr);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "vertex");

        // compile fragment shader
        auto fragmentOpt{ util::StringFromFile(fragmentPath) };
        if (fragmentOpt.has_value() == false) {
            throw ShaderError(std::string("failed to load fragment shader ") + std::string(fragmentPath));
        }
        auto fragmentShader{ fragmentOpt.value() };

        GLuint fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
        const GLchar *fragmentCode{ fragmentShader.c_str() };
        glShaderSource(fragment, 1, &fragmentCode, nullptr);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "fragment");

        // link shader Program
        _id = glCreateProgram();
        glAttachShader(_id, vertex);
        glAttachShader(_id, fragment);
        glLinkProgram(_id);
        CheckCompileErrors(_id, "program");

        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::Use()
    {
        glUseProgram(_id);
    }

    static inline bool CheckLocation(GLint location, const char *name)
    {
        if constexpr (debugging) {
            // confirm legitimate location
            if (location == -1) {
                printf("could not find gl uniform '%s'\n", name);
                return false;
            }
        }

        return true;
    }

    static bool GLErrorCheck(const char *name)
    {
        if constexpr (debugging) {
            switch (glGetError()) {
                case GL_INVALID_OPERATION: {
                    printf("failed to set uniform '%s'\n", name);
                    return false;
                } break;

                case GL_INVALID_VALUE: {
                    printf("error while setting uniform '%s'\n", name);
                    return false;
                } break;

                case GL_NO_ERROR: {
                    return true;
                } break;

                default: {
                    printf("unknown opengl error when setting unform\n");
                    return false;
                } break;
            }
        }
        else {
            return true;
        }
        return false;
    }

    bool Shader::Set(const char *name, const bool &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform1i(location, static_cast<int>(value));

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const int &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform1i(location, value);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const float &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform1f(location, value);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::vec2 &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform2fv(location, 1, &value[0]);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::vec3 &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform3fv(location, 1, &value[0]);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::vec4 &value) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniform4fv(location, 1, &value[0]);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::mat2 &mat) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::mat3 &mat) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);

        return GLErrorCheck(name);
    }

    bool Shader::Set(const char *name, const glm::mat4 &mat) const
    {
        auto location{ glGetUniformLocation(_id, name) };
        if (!CheckLocation(location, name)) {
            return false;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);

        return GLErrorCheck(name);
    }

    GLint Shader::GetAttribLocation(const char *name)
    {
        return glGetAttribLocation(_id, name);
    }

    void Shader::CheckCompileErrors(const GLuint &shader, const char *type)
    {
        GLint success{};
        GLchar infoLog[1024]{};
        if (strcmp(type, "program") != 0) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                throw std::runtime_error(std::string("shader compile fail: ") + std::string(type) + " because of " + std::string(infoLog));
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                throw std::runtime_error(std::string("shader link fail: ") + type + " " + infoLog);
            }
        }
    }

} // namespace Wrappers