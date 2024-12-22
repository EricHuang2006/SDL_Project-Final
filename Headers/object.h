#pragma once

#include "Window.h"
#include<SDL_image.h>
#include<vector>

class Obj{
public:
    Obj(int _x, int _y, int _type);
    ~Obj();
    void draw();
    int x, y, type;

private:
    SDL_Texture *icon = nullptr;

};

