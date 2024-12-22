#include "Headers/cross.h"
#include <string>
#include <iostream>
#include <vector>
#define cross_image_path "Images/cross.png"
#define pb push_back


Cross::Cross(int _x, int _y) : x(_x), y(_y), cd(ct + 800), dx(0), dy(0){
    /*
    string path = "Images/cross.jpg";
    SDL_Surface *surface = IMG_Load(path.c_str());
    if(!surface){
        cerr<<"Failed to create surface.\n";
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!icon){
        cerr<<"Failed to create texture.\n";
    }
    SDL_FreeSurface(surface);
    */
}

Cross::~Cross(){
    SDL_DestroyTexture(icon);
}

