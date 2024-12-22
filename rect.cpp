#pragma once

#include"Headers/rect.h"
#include"Headers/window.h"
#include<string>
#include<iostream>
using namespace std;


Rect::Rect(int _x, int _y, int _w, int _h, const string& image_path): x(_x), y(_y), w(_w), h(_h) {
    SDL_Surface* surface = IMG_Load(image_path.c_str());
    if (!surface) {
        cerr << "Failed to create rect " << endl;
        return;
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_FreeSurface(surface);
};
Rect::~Rect() {
    SDL_DestroyTexture(icon);
}

void Rect::draw() {
    SDL_Rect rect = {x, y, w, h};
    if(icon){
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
}
void Rect::setAlpha(Uint8 alpha) {
    if (icon) {
        SDL_SetTextureAlphaMod(icon, alpha);
    }
}