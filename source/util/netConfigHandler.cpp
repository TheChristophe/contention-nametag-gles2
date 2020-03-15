#include "netConfigHandler.hpp"

#include "animationController.hpp"

#include <nlohmann/json.hpp>

#include <fcntl.h>
#include <sys/stat.h>

#include <string>

static struct mq_attr queueConfig {
    .mq_flags   = 0,
    .mq_maxmsg  = 16,
    .mq_msgsize = 4096,
    .mq_curmsgs = 0
};

NetConfigHandler::NetConfigHandler(AnimationController *controller)
    : _controller(controller)
    , _queue(mq_open("/nametag", O_RDONLY | O_CREAT | O_NONBLOCK, S_IRWXU, &queueConfig))
{
    if (_queue == static_cast<mqd_t>(-1)) {
        throw std::runtime_error(std::string("failed to open message queue: ") + strerror(errno));
    }
}

NetConfigHandler::~NetConfigHandler()
{
    mq_close(_queue);
    // kill the queue here because we are the receiving end
    mq_unlink("/nametag");
}

void NetConfigHandler::Poll()
{
    ssize_t size{ mq_receive(_queue, _buffer, sizeof(_buffer) - 1, nullptr) };
    while (size != -1) {
        using json = nlohmann::json;

        _buffer[4096] = '\0';
        puts(_buffer);

        auto data = json::parse(_buffer);

        if (data["type"] == "post") {
            if (data["content"]["type"] == "triangle") {
                auto x = std::stof(data["content"]["x"].get<std::string>());
                auto y = std::stof(data["content"]["y"].get<std::string>());
                _controller->AddTriangle(x, y);
            }
        }
        else if (data["type"] == "put") {
        }
        else if (data["type"] == "get") {
        }

        size = mq_receive(_queue, _buffer, sizeof(_buffer) - 1, nullptr);
    }
}
