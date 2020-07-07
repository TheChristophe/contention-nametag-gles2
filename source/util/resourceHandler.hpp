#ifndef RESOURCE_HANDLER_HPP
#define RESOURCE_HANDLER_HPP

#include "wrappers/shader.hpp"

#include <map>
#include <memory>
#include <string>
#include <type_traits>

struct ResourceHandlerConfig;

class ResourceHandler {
    public:
    ResourceHandler(int screenWidth, int screenHeight);
    ~ResourceHandler();
    std::shared_ptr<Wrappers::Shader> LoadShader(const std::string &identifier);

    private:
    std::map<std::string_view, std::shared_ptr<Wrappers::Shader>> _shaders;
    const int _width;
    const int _height;
    std::unique_ptr<ResourceHandlerConfig> _;
};

#endif