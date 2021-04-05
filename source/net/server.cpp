#include "server.hpp"

#include "util/animationController.hpp"

#include <fstream>

std::string LoadFile(const char *filename)
{
    std::ifstream t(filename);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

WebServer::WebServer(AnimationController &controller)
    : _controller(controller)
    , _page(LoadFile("res/static/index.html"))
    , _css(LoadFile("res/static/main.css"))
    , _js(LoadFile("res/static/ui.js"))
{
}

enum RequestType {
    POST = 1,
    PUT  = 2,
    GET  = 3
};

nlohmann::json WebServer::Handle(int type, const nlohmann::json &metadata)
{
    using json = nlohmann::json;
    if (type == RequestType::POST) {
        if (metadata["type"] == "triangle") {
            auto &data = metadata;
            auto x{ std::stof(data["x"].get<std::string>()) };
            auto y{ std::stof(data["y"].get<std::string>()) };
            auto id{ _controller.ReqAddTriangle(x, y) };

            return json{
                { "type", "triangle" },
                { "id", id.get() }
            };
        }
        else if (metadata["type"] == "text") {
            auto &data = metadata;
            auto x{ std::stof(data["x"].get<std::string>()) };
            auto y{ std::stof(data["y"].get<std::string>()) };
            auto text{ data["text"].get<std::string>() };
            bool wavy{ data.contains("wavy") };
            auto id{ _controller.ReqAddText(text, wavy, x, y) };

            return json{
                { "type", "text" },
                { "id", id.get() }
            };
        }
    }
    else if (type == RequestType::PUT) {
        if (metadata["type"] == "_delete") {
            auto &data = metadata;
            auto id{ data["id"].get<int>() };
            _controller.Remove(id);
        }
    }
    else if (type == RequestType::GET) {
    }
    return json{};
}

void WebServer::Run()
{
    uWS::App()
        .get("/", [this](auto *res, auto *req) {
            res->end(_page.c_str());
        })
        .get("/index.html", [this](auto *res, auto *req) {
            res->end(_page.c_str());
        })
        .get("/css", [this](auto *res, auto *req) {
            res->end(_css.c_str());
        })
        .get("/js", [this](auto *res, auto *req) {
            res->end(_js.c_str());
        })
        .post("/add", [this](auto *res, auto *req) {
            res->onData([res, this](std::string_view data, bool last) {
                std::string buffer{};
                if (not last) {
                    res->writeStatus("400 Bad Request");
                    res->end("<!DOCTYPE html><html lang=\"en\"><body>error</body></html>");
                }
                buffer.append(data.data(), data.size());

                auto out = this->Handle(RequestType::POST, nlohmann::json::parse(buffer)).dump();

                res->writeStatus("200 OK");
                res->writeHeader("content-type", "application/json");

                res->end(out);
            });
            res->onAborted([]() {
                // dunno
                std::cout << "aborted data" << std::endl;
            });
            //res->end(Handle().dump());
        })
        .put("/delete", [this](auto *res, auto *req) {
            res->onData([res, this](std::string_view data, bool last) {
                std::string buffer{};
                if (not last) {
                    res->writeStatus("400 Bad Request");
                    res->end("<!DOCTYPE html><html lang=\"en\"><body>error</body></html>");
                }
                buffer.append(data.data(), data.size());

                auto out = this->Handle(RequestType::PUT, nlohmann::json::parse(buffer)).dump();

                res->writeStatus("200 OK");
                res->writeHeader("content-type", "application/json");

                res->end(out);
            });
            res->onAborted([]() {
                // dunno
                std::cout << "aborted data" << std::endl;
            });
            //res->end(Handle().dump());
        })
        .listen(_port, [this](auto *token) {
            this->_socket = token;
            if (token) {
                // success
            }
        })
        .run();
}

void WebServer::Halt()
{
    us_listen_socket_close(0, _socket);
}
