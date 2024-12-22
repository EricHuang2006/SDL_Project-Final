#pragma once

#include<SDL_ttf.h>
#include<graphics.h>
#include<string>
using namespace std;

class Text{
public:
    Text(SDL_Renderer *renderer, const string &font_path, int font_size, const Uint16 *msg_text, const SDL_Color &color);
    Text(SDL_Renderer *renderer, const string &font_path, int font_size, const string *msg_text, const SDL_Color &color);
    void display(int x, int y, SDL_Renderer *renderer) const;
    void setAlpha(Uint8 alpha);
    void cleanup();
    static SDL_Texture *loadFont(SDL_Renderer *renderer, const string &font_path, int font_size, const Uint16 *msg_text, const SDL_Color &color);
    static SDL_Texture *loadFont2(SDL_Renderer *renderer, const string &font_path, int font_size, const string *msg_text, const SDL_Color &color);
private:
    SDL_Texture *text_texture = nullptr;
    mutable SDL_Rect text_rect;
	SDL_Renderer *renderer = nullptr;
};