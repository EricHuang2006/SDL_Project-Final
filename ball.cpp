#include "Headers/ball.h"
#include <string>
#include <iostream>
#include <vector>
#define ball_image_path "Images/ball.png"
#define pb push_back

vector<Ball*> ball_list;

Ball::Ball(int _x, int _y) : x(_x), y(_y), cd(ct + 1000){
    string path = ball_image_path;
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
Ball::~Ball(){
    SDL_DestroyTexture(icon);
}
void Ball::draw(){
    SDL_Rect rect = {x, y, 50, 50};
    if(icon){
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
}
void put_ball(int x, int y){
    ball_list.pb(new Ball(x, y));
}