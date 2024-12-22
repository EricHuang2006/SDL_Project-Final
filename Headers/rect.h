#pragma once

#include<SDL_image.h>
#include<string>
using namespace std;

class Rect{
public:
    Rect(int _x, int _y, int _w, int _h, const string& image_path);
    ~Rect();
    void setAlpha(Uint8 alpha);
    void draw();
private:
    int x, y, w, h;
    SDL_Texture* icon = nullptr;
};