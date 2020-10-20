#include "resourceHandler.hpp"

#include <cpptoml.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

struct ResourceHandlerConfig {
    std::shared_ptr<cpptoml::table> config{};
    GLuint _bayerTexture{};
};

void SanityCheck(const std::shared_ptr<cpptoml::table> &config)
{
    if (!config->get_table("shaders").operator bool()) {
        fprintf(stderr, "missing shaders section in config\n");
        throw std::runtime_error("resourceHandler: incomplete config");
    }
}

ResourceHandler::ResourceHandler(int screenWidth, int screenHeight)
    : _width{ screenWidth }
    , _height{ screenHeight }
    , _{ new ResourceHandlerConfig{ cpptoml::parse_file("configuration.toml"), {} } }
{
    SanityCheck(_->config);
    uint8_t fourByFourBayer[4][4]{
        { 0, 8, 2, 10 },
        { 12, 4, 14, 6 },
        { 3, 11, 1, 9 },
        { 15, 7, 13, 5 }
    };

    glGenTextures(1, &_->_bayerTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _->_bayerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 4, 4, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, fourByFourBayer);
}

// required defining here for ResourceHandlerConfig unique_ptr
ResourceHandler::~ResourceHandler() = default;

std::shared_ptr<Wrappers::Shader> ResourceHandler::LoadShader(const std::string &identifier)
{
    // return if it is loaded
    if (auto it{ _shaders.find(identifier) }; it != _shaders.end()) {
        return it->second;
    }

    auto shaders{ _->config->get_table("shaders") };

    if (!shaders->get_qualified(identifier + ".fragment").operator bool() || !shaders->get_qualified(identifier + ".vertex").operator bool()) {
        fprintf(stderr, "shader entry for %s malformed\n", identifier.c_str());
        return nullptr;
    }

    auto fragment{ *shaders->get_qualified_as<std::string>(identifier + ".fragment") };
    auto vertex{ *shaders->get_qualified_as<std::string>(identifier + ".vertex") };

    auto pointer{ std::make_shared<Wrappers::Shader>(vertex, fragment) };

    pointer->Use();

    // setup shader
    auto viewMatrix{ glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f)) };
    auto projectionMatrix{ glm::perspective(45.f, static_cast<float>(_width) / static_cast<float>(_height), 0.1f, 100.0f) };
    pointer->Set("projection", projectionMatrix * viewMatrix);
    pointer->Set("time", 0.f);
    pointer->Set("bayerTex", 0);
    pointer->Set("offset", glm::vec2(0, 0));

    _shaders.insert({ identifier, pointer });
    return pointer;
}
