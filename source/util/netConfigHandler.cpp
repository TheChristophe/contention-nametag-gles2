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
    , _inQueue(mq_open("/nametag", O_RDONLY | O_CREAT | O_NONBLOCK, S_IRWXU, &queueConfig))
    , _outQueue(mq_open("/nametag-out", O_WRONLY | O_CREAT | O_NONBLOCK, S_IRWXU, &queueConfig))
{
    if (_inQueue == static_cast<mqd_t>(-1)) {
        throw std::runtime_error(std::string("failed to open message queue: ") + strerror(errno));
    }
}

NetConfigHandler::~NetConfigHandler()
{
    mq_close(_inQueue);
    // kill the queue here because we are the receiving end
    mq_unlink("/nametag");
    mq_close(_outQueue);
    // kill the queue here because we are the receiving end
    mq_unlink("/nametag-out");
}

void NetConfigHandler::Poll()
{
    ssize_t size{ mq_receive(_inQueue, _buffer, sizeof(_buffer) - 1, nullptr) };
    while (size != -1) {
        using json = nlohmann::json;

        _buffer[4096] = '\0';
        puts(_buffer);

        auto metadata = json::parse(_buffer);

        Handle(metadata);

        size = mq_receive(_inQueue, _buffer, sizeof(_buffer) - 1, nullptr);
    }
}

void NetConfigHandler::Handle(const nlohmann::json &metadata)
{
    using json = nlohmann::json;

    if (metadata["type"] == "post") {
        if (metadata["content"]["type"] == "triangle") {
            auto &data = metadata["content"];
            auto x{ std::stof(data["x"].get<std::string>()) };
            auto y{ std::stof(data["y"].get<std::string>()) };
            auto id{ _controller->AddTriangle(x, y) };

            Respond(json{
                { "uuid", metadata["uuid"] },
                { "id", id } });
        }
    }
    else if (metadata["type"] == "put") {
        if (metadata["content"]["type"] == "_delete") {
            auto &data = metadata["content"];
            auto id{ data["id"].get<int>() };
            _controller->Remove(id);
        }
    }
    else if (metadata["type"] == "get") {
    }
}

void NetConfigHandler::Respond(const nlohmann::json &response)
{
    auto str = response.dump();
    if (mq_send(_outQueue, str.c_str(), str.length() + 1, 0) == -1) {
        switch (errno) {
            case EAGAIN: {
                // trying to send message before webserver has finished reading them
                std::fprintf(stderr, "Unexpected condition (%s:%i)", __FILE__, __LINE__);
                std::exit(1);
            } break;
            case EINTR: {
                // sending was interrupted by signal (how to respond to this?)
                std::fprintf(stderr, "Unexpected condition (%s:%i)", __FILE__, __LINE__);
                std::exit(1);
            } break;
            case EBADF: {
                // invalid file descriptor, try recreating
                mq_close(_outQueue);
                _outQueue = mq_open("/nametag-out", O_WRONLY | O_CREAT | O_NONBLOCK, S_IRWXU, &queueConfig);
            } break;
        }
    }
}