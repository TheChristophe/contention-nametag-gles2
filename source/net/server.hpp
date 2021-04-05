#ifndef SERVER_HPP
#define SERVER_HPP

#include "util/animationController.hpp"

#include <App.h>
#include <libusockets.h>
#include <nlohmann/json.hpp>

#include <string>

class WebServer {
    public:
    explicit WebServer(class AnimationController &controller);
    ~WebServer() = default;

    nlohmann::json Handle(int type, const nlohmann::json &metadata);

    void Run();
    void Halt();

    private:
    us_listen_socket_t *_socket{};

    AnimationController &_controller;

    short _port{ 8080 };
};

#endif