#ifndef NET_CONFIG_HANDLER_HPP
#define NET_CONFIG_HANDLER_HPP

#include <nlohmann/json_fwd.hpp>

#include <mqueue.h> //mqd_t

#include <cstdint>

class NetConfigHandler {
    public:
    NetConfigHandler(class AnimationController *controller);
    ~NetConfigHandler();

    void Poll();

    private:
    void Handle(const nlohmann::json &metadata);
    void Respond(const nlohmann::json &response);
    class AnimationController *_controller;
    mqd_t _inQueue;
    mqd_t _outQueue;
    char _buffer[4097];
};

#endif