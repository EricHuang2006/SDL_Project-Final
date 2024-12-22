#include "Headers/object.h"
#include <string>
#include <iostream>
#include <vector>
#define pb push_back


Obj::Obj(int _x, int _y, int _type) : x(_x), y(_y), type(_type){
    string path = "Images/object" + string(1, char(type + '0')) + ".png";
    SDL_Surface *surface = IMG_Load(path.c_str());
    if(!surface){
        cerr<<"Failed to create surface.\n";
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!icon){
        cerr<<"Failed to create texture.\n";
    }
    SDL_FreeSurface(surface);
}

Obj::~Obj(){
    SDL_DestroyTexture(icon);
}

void Obj::draw(){
    SDL_Rect rect = {x, y, 50, 50};
    if(icon){
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
}