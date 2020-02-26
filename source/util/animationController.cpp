#include "animationController.hpp"

#include "drawers/gl/decoration.hpp"
#include "drawers/gl/drawable.hpp"
#include "drawers/gl/fonts.hpp"

#include <iostream>

AnimationController::AnimationController(int width, int height)
    : _resources(width, height)
    , _width{ width }
    , _height{ height }
{
}

AnimationController::identifier AnimationController::AddTriangle()
{
    auto p  = new Drawers::GL::Decoration(_resources.LoadShader("triangle"));
    auto id = _nextID++;
    _drawables.emplace(id, dynamic_cast<Drawers::GL::Drawable *>(p));
    return id;
}

AnimationController::identifier AnimationController::AddText(const char *text)
{
    auto p  = new Drawers::GL::Fonts(_resources.LoadShader("text"), _width, _height, text);
    auto id = _nextID++;
    _drawables.emplace(id, dynamic_cast<Drawers::GL::Drawable *>(p));
    return id;
}

void AnimationController::Remove(AnimationController::identifier id)
{
    if (auto it = _drawables.find(id); it != _drawables.end()) {
        _drawables.erase(it);
    }
}

void AnimationController::Draw(float time)
{
    for (auto &[id, drawable] : _drawables) {
        drawable->Draw(time);
    }
}