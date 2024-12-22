#pragma once

#include "Window.h"
#include<SDL_image.h>
#include<vector>

class Ball{
public:
    Ball(int _x, int _y);
    ~Ball();
    void draw();
    int x, y, cd;
    
private:
    SDL_Texture *icon = nullptr;

};

void put_ball(int x, int y);

extern vector<Ball*> ball_list;