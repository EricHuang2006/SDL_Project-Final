#include "Headers/text.h"
#include "Headers/window.h"
#include<iostream>

Text::Text(SDL_Renderer *renderer, const string &font_path, int font_size, 
    const string* msg_text, const SDL_Color &color):renderer(renderer){
    text_texture = loadFont2(renderer, font_path, font_size, msg_text, color);
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
}

Text::Text(SDL_Renderer *renderer, const string &font_path, int font_size, 
    const Uint16* uni_text, const SDL_Color &color):renderer(renderer){
    text_texture = loadFont(renderer, font_path, font_size, uni_text, color);
    SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
}

void Text::display(int x, int y, SDL_Renderer *renderer) const {
    if (text_texture) {
        text_rect.x = x;
        text_rect.y = y;
        SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
    }
}

void Text::cleanup() {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }
}

void Text::setAlpha(Uint8 alpha) {
    if (text_texture) {
        SDL_SetTextureAlphaMod(text_texture, alpha); // Adjust the alpha value
    }
}

SDL_Texture *Text::loadFont(SDL_Renderer *renderer, const string &font_path, int font_size, const Uint16 *msg_text, const SDL_Color &color) {
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return nullptr;
    }

    SDL_Surface *text_surface = TTF_RenderUNICODE_Blended(font, msg_text, color);
    if (!text_surface) {
        cerr << "Failed to create text surface: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(font);

    if (!texture) {
        cerr << "Failed to create text texture: " << SDL_GetError() << endl;
    }

    return texture;
}

SDL_Texture *Text::loadFont2(SDL_Renderer *renderer, const string &font_path, int font_size, const string *msg_text, const SDL_Color &color) {
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return nullptr;
    }

    SDL_Surface *text_surface = TTF_RenderText_Blended(font, msg_text->c_str(), color);
    if (!text_surface) {
        cerr << "Failed to create text surface: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(font);

    if (!texture) {
        cerr << "Failed to create text texture: " << SDL_GetError() << endl;
    }
    return texture;
}