#pragma once

#include "window.h"
#include<SDL_image.h>
#include<vector>

class item{
public:
    item(int _x, int _y, int _type);
    ~item();
    void draw();
    int x, y, type;
    virtual int heal(){ return 0;};
    virtual int get_shield_cd(){ return 0;};
private:
    SDL_Texture *icon = nullptr;

};

class healing_potion : public item {
public:
    healing_potion(int _x, int _y): item(_x,_y, 0), heal_amount(600) {};
    ~healing_potion()= default;
    int heal() override {return heal_amount;};
private:
    int heal_amount;
};

class shield: public item {
public:
    shield(int _x, int _y): item (_x, _y, 1), shield_cd(800) {};
    ~shield() = default;
    int get_shield_cd() override {return shield_cd;};
private:
    int shield_cd;
};