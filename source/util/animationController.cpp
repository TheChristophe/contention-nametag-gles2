#include "animationController.hpp"

#include "drawers/gl/drawable.hpp"
#include "drawers/gl/fonts.hpp"
#include "drawers/gl/sprite.hpp"
#include "drawers/gl/triangle.hpp"

#include <iostream>

AnimationController::AnimationController(int width, int height)
    : _resources(width, height)
    , _width{ width }
    , _height{ height }
{
}

std::future<AnimationController::identifier> AnimationController::ReqAddTriangle(float x, float y)
{
    auto &r  = _requests.emplace_back();
    r.lambda = [this, x, y, &r]() {
        r.promise.set_value(this->AddTriangle(x, y));
    };
    return r.promise.get_future();
}

std::future<AnimationController::identifier> AnimationController::ReqAddText(std::string text, bool wavy, float x, float y)
{
    auto &r  = _requests.emplace_back();
    r.lambda = [this, text, wavy, x, y, &r]() {
        r.promise.set_value(this->AddText(text.c_str(), wavy, x, y));
    };
    return r.promise.get_future();
}

AnimationController::identifier AnimationController::AddTriangle(float x, float y)
{
    auto p  = new Drawers::GL::Triangle(_resources.LoadShader("triangle"));
    auto id = _nextID++;
    p->MoveTo(glm::vec2(x, y));
    _drawables.emplace(id, dynamic_cast<Drawers::GL::Drawable *>(p));
    return id;
}

AnimationController::identifier AnimationController::AddText(const char *text, bool wavy, float x, float y)
{
    auto p  = new Drawers::GL::Fonts(_resources.LoadShader("text"), _width, _height, text);
    auto id = _nextID++;
    p->MoveTo(glm::vec2(x, y));
    p->SetWavy(wavy);
    _drawables.emplace(id, dynamic_cast<Drawers::GL::Drawable *>(p));
    return id;
}

AnimationController::identifier AnimationController::AddSprite(std::filesystem::path file, bool transparent, float x, float y)
{
    auto p  = new Drawers::GL::Sprite(_resources.LoadShader("sprite"), file, transparent);
    auto id = _nextID++;
    p->MoveTo(glm::vec2(x, y));
    _drawables.emplace(id, dynamic_cast<Drawers::GL::Drawable *>(p));
    return id;
}

Drawers::GL::Drawable *AnimationController::GetDrawable(AnimationController::identifier id)
{
    if (auto it = _drawables.find(id); it != _drawables.end()) {
        return it->second.get();
    }
    return nullptr;
}

void AnimationController::Remove(AnimationController::identifier id)
{
    if (auto it = _drawables.find(id); it != _drawables.end()) {
        _drawables.erase(it);
    }
}

void AnimationController::ProcessRequests()
{
    for (auto &r : _requests) {
        r.lambda();
    }
    _requests.clear();
}

void AnimationController::Draw(float time)
{
    for (auto &[id, drawable] : _drawables) {
        drawable->Draw(time);
    }
}