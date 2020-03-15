#ifndef NET_CONFIG_HANDLER_HPP
#define NET_CONFIG_HANDLER_HPP

#include <mqueue.h> //mqd_t

#include <cstdint>

class NetConfigHandler {
    public:
    NetConfigHandler(class AnimationController *controller);
    ~NetConfigHandler();

    void Poll();

    private:
    class AnimationController *_controller;
    mqd_t _queue;
    char _buffer[4097];
};

#endif