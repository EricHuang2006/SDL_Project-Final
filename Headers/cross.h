#pragma once

#include "Window.h"
#include<SDL_image.h>
#include<vector>

class Cross{
public:
    Cross(int _x, int _y);
    ~Cross();
    void draw();
    int x, y, cd;
    friend void deal_damage(Cross*);
    vector<SDL_Rect> dx, dy;

private:
    SDL_Texture *icon = nullptr;

};
