#include "resourceHandler.hpp"

#include <cpptoml.h>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <type_traits>

struct ResourceHandlerConfig {
    std::shared_ptr<cpptoml::table> config;
};

void SanityCheck(const std::shared_ptr<cpptoml::table> &config)
{
    if (!config->get_table("shaders").operator bool()) {
        fprintf(stderr, "missing shaders section in config\n");
        throw std::runtime_error("resourceHandler: incomplete config");
    }
}

ResourceHandler::ResourceHandler()
    : _{ new ResourceHandlerConfig{ cpptoml::parse_file("configuration.toml") } }
{
    SanityCheck(_->config);
}

ResourceHandler::~ResourceHandler()
{
}

std::shared_ptr<Wrappers::Shader> ResourceHandler::LoadShader(const std::string &identifier)
{
    // return if it is loaded
    if (auto it = _shaders.find(identifier); it != _shaders.end()) {
        return it->second;
    }

    auto shaders = _->config->get_table("shaders");
    //std::cout << _ << " " << _->config.operator bool() << " " << _->config->is_value() << " " << shader << std::endl;

    if (!shaders->get_qualified(identifier + ".fragment").operator bool() || !shaders->get_qualified(identifier + ".vertex").operator bool()) {
        fprintf(stderr, "shader entry for %s malformed\n", identifier.c_str());
        return nullptr;
    }

    auto fragment = *shaders->get_qualified_as<std::string>(identifier + ".fragment");
    auto vertex   = *shaders->get_qualified_as<std::string>(identifier + ".vertex");

    auto pointer = std::make_shared<Wrappers::Shader>(vertex, fragment);

    _shaders.insert({ identifier, pointer });
    return pointer;
}