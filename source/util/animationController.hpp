#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP

#include "drawers/drawable.hpp"
#include "util/resourceHandler.hpp"

#include <deque>
#include <filesystem>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <string>

class AnimationController {
    public:
    using identifier = int;
    AnimationController(int width, int height);
    std::future<identifier> ReqAddTriangle(float x = 0.f, float y = 0.f);
    std::future<identifier> ReqAddText(const std::string& text, bool wavy = false, float x = 0.f, float y = 0.f);
    std::future<identifier> ReqAddSprite(const std::filesystem::path& file, bool transparent = false, /*bool wavy = false, */ float x = 0.f, float y = 0.f);
    identifier AddTriangle(float x = 0.f, float y = 0.f);
    identifier AddText(const char *text, bool wavy = false, float x = 0.f, float y = 0.f);
    identifier AddSprite(const std::filesystem::path& file, bool transparent = false, /*bool wavy = false, */ float x = 0.f, float y = 0.f);
    Drawers::Drawable *GetDrawable(identifier id);
    void Remove(identifier id);

    void ProcessRequests();
    void Draw(float time);

    private:
    std::map<identifier, std::unique_ptr<Drawers::Drawable>> _drawables;
    identifier _nextID{ 0 };
    ResourceHandler _resources;
    int _width;
    int _height;

    class Request {
        public:
        std::function<void()> lambda;
        std::promise<AnimationController::identifier> promise;
    };

    std::deque<Request> _requests;
};

#endif