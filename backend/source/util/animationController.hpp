#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP

#include "drawers/gl/drawable.hpp"
#include "util/resourceHandler.hpp"

#include <filesystem>
#include <map>
#include <memory>

class AnimationController {
    public:
    using identifier = int;
    AnimationController(int width, int height);
    identifier AddTriangle(float x = 0.f, float y = 0.f);
    identifier AddText(const char *text, bool wavy = false, float x = 0.f, float y = 0.f);
    identifier AddSprite(std::filesystem::path file, bool transparent = false, /*bool wavy = false, */ float x = 0.f, float y = 0.f);
    Drawers::GL::Drawable *GetDrawable(identifier id);
    void Remove(identifier id);

    void Draw(float time);

    private:
    std::map<identifier, std::unique_ptr<Drawers::GL::Drawable>> _drawables;
    identifier _nextID{ 0 };
    ResourceHandler _resources;
    int _width;
    int _height;
};

#endif