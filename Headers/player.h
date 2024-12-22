#pragma once

#include "window.h"
#include "object.h"
#include "item.h"
#include<string>
#include<vector>
#include<iterator>
#include<algorithm>

class health_bar{
public:
    health_bar(){}
    health_bar(int _pos) : pos(_pos), h(3000){}
    void draw();
    int pos; // left or right
    bool operator > (int amount);
    bool operator < (int amount);
    bool operator == (int amount);
    bool operator <= (int amount);
    bool operator >= (int amount);
    int operator +(int amount);
    health_bar& operator -= (int amount);
    health_bar& operator += (int amount);
    health_bar& operator = (int amount);
private:
    int h;
};

class symbol{
public:
    symbol(){};
    symbol(int _x,int _y);
    void draw();
    void move(int _x,int _y);
    int x, y, w, h, scd;
    ~symbol();
    SDL_Texture *icon = nullptr;
};

class Player{
public:
    Player(int _x, int _y, int _w, int _h, vector<int> _keys, const string& image_path, const string& soundPath);
    Player();
    ~Player();
    void AI();
    void draw();
    void PollEvents(SDL_Event);

    bool loadSoundEffect(const string& soundPath);
    void playSoundEffect();
    void cleanupAudio();
    Mix_Chunk* soundEffect = nullptr;

    int x, y, w, h, cd, scd;
    bool have_shield, use_shield;
    health_bar health;
    symbol symbol;
    friend bool Collision(const Player *a, const Player *b);
    friend bool Collision2(const Player *a, const Obj *b);
    friend bool Collision3(const Player *a, const SDL_Rect &b);
    friend bool Collision4(const Player *a, const item *b);
private:
    int r, g, b, a;
    vector<int> keys;
    SDL_Texture *icon = nullptr;
};

extern bool valid_move(Player*);
extern int dx[];
extern int dy[];