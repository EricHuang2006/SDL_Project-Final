#include "Headers/player.h"
#include "Headers/ball.h"
#include "Headers/object.h"
#include "Headers/item.h"
#include<graphics.h>
#include<SDL_image.h>
#include<iostream>
using namespace std;


Player::Player(){}
Player::Player(int _x, int _y, int _w, int _h, vector<int> _keys, const string &image_path, const string& sound_path)
    : x(_x), y(_y), w(_w), h(_h), keys(_keys), health(_x > 400), scd(0), symbol(_x,_y), have_shield(false), use_shield(0){
    cd = ct;
    SDL_Surface *surface = IMG_Load(image_path.c_str());
    loadSoundEffect(sound_path.c_str());
    if(!surface){
        cerr<<"Failed to create surface.\n";
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!icon){
        cerr<<"Failed to create texture.\n";
    }
    SDL_FreeSurface(surface);
}

Player::~Player(){
    SDL_DestroyTexture(icon);
}
int dx[] = {-50, 50, 0, 0}; // [L, R, U, D]
int dy[] = {0, 0, -50, 50};

bool Player::loadSoundEffect(const string& soundPath) {
    soundEffect = Mix_LoadWAV(soundPath.c_str());
    if (!soundEffect) {
        cerr << "Failed to load sound effect: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}

void Player::playSoundEffect() {
    if (soundEffect) {
        Mix_PlayChannel(-1, soundEffect, 0);
    }
}

void Player::cleanupAudio() {
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
        soundEffect = nullptr;
    }
    Mix_CloseAudio();
}

void Player::PollEvents(SDL_Event event){
    if(event.type == SDL_KEYDOWN){
        int op = event.key.keysym.sym;
        int id = find(keys.begin(), keys.end(), op) - keys.begin();
        if(id == keys.size()) return;
        if(id == 4 || id == 5){
            if(cd > ct) return;
            cd = ct + 600;
            cout<<"place a ball on ("<<x<<", "<<y<<")\n";
            playSoundEffect();
            put_ball(x, y);
        }
        else{
            x += dx[id], y += dy[id];
            if(!valid_move(this)) x -= dx[id], y -= dy[id];
            symbol.move(x, y);
        }
    }
}

void health_bar::draw(){
    SDL_Rect rect;
    if(!pos) rect = {10, 10, h / 11, 35};
    else rect = {800 - 10 - h / 11, 10, h / 11, 35};
    SDL_SetRenderDrawColor(Window::renderer, 254, 32, 32, 128);
    SDL_RenderFillRect(Window::renderer, &rect);
}

void Player::draw(){
    SDL_Rect rect = {x, y, w, h};
    if(icon){
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
    else{
        SDL_SetRenderDrawColor(Window::renderer, r, g, b, a);
        SDL_RenderFillRect(Window::renderer, &rect);
    }
    health.draw();
    if(have_shield) symbol.draw();
}

bool Collision(const Player *a, const Player *b){
    SDL_Rect ta = {a->x, a->y, a->w, a->h};
    SDL_Rect tb = {b->x, b->y, b->w, b->h};
    return SDL_HasIntersection(&ta, &tb);
}

bool Collision2(const Player *a, const Obj *b){
    SDL_Rect ta = {a->x, a->y, a->w, a->h};
    SDL_Rect tb = {b->x, b->y, 50, 50};
    return SDL_HasIntersection(&ta, &tb);
}

bool Collision3(const Player *a, const SDL_Rect &b){
    SDL_Rect ta = {a->x, a->y, a->w, a->h};
    return SDL_HasIntersection(&ta, &b);
}

bool Collision4(const Player *a, const item *b){
    SDL_Rect ta = {a->x, a->y, a->w, a->h};
    SDL_Rect tb = {b->x, b->y, 50, 50};
    return SDL_HasIntersection(&ta, &tb);
}

symbol::symbol(int _x,int _y) : x(_x+30),y(_y), w(19), h(21), scd(0) {
    string image_path = "images/shield.png";
    SDL_Surface *surface = IMG_Load(image_path.c_str());
    if(!surface){
        cerr<<"Failed to create surface.\n";
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    if(!icon){
        cerr<<"Failed to create texture.\n";
    }
    SDL_FreeSurface(surface);
};
void symbol::draw() {
    SDL_Rect rect = {x, y, w, h};
    if(icon){
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
}
void symbol::move(int _x, int _y){
    x = _x+30, y = _y;
}
symbol::~symbol() {
    SDL_DestroyTexture(icon);
};

health_bar& health_bar::operator+=(int amount) {
    h+=amount;
    return *this;
}
health_bar& health_bar::operator-=(int amount) {
    h-=amount;
    return *this;
}
health_bar& health_bar::operator=(int amount) {
    h=amount;
    return *this;
}
bool health_bar::operator>(int amount) {
    return h>amount;
}
bool health_bar::operator<(int amount) {
    return h<amount;
}
bool health_bar::operator==(int amount) {
    return h==amount;
}
bool health_bar::operator<=(int amount) {
    return h<=amount;
}
bool health_bar::operator>=(int amount) {
    return h>=amount;
}
int health_bar::operator+(int amount) {
    return h+amount;
}