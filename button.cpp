#include "Headers/button.h"
#include "Headers/window.h"
#include <iostream>
using namespace std;

Button::Button(int _x, int _y, int _w, int _h, const string& image_path) : x(_x), y(_y), width(_w), height(_h) {
    SDL_Surface* surface = IMG_Load(image_path.c_str());
    if (!surface) {
        cerr << "Failed to create button " << endl;
        return;
    }
    icon = SDL_CreateTextureFromSurface(Window::renderer, surface);
    SDL_FreeSurface(surface);
}

Button::~Button() {
    if (icon) {
        SDL_DestroyTexture(icon);
    }
}

void Button::draw() {
    SDL_Rect rect = {x, y, width, height};

    if (isHovered) {
        SDL_SetRenderDrawColor(Window::renderer, 255, 255, 0, 100);
        SDL_RenderFillRect(Window::renderer, &rect);
    }

    if (icon) {
        SDL_RenderCopy(Window::renderer, icon, nullptr, &rect);
    }
}

bool Button::PollEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        SDL_Rect rect = {x, y, width, height};
        if (mouseX >= rect.x and mouseX <= rect.x + rect.w and
            mouseY >= rect.y and mouseY <= rect.y + rect.h) {
            isHovered = true;
        }
		else {
            isHovered = false;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        SDL_Rect rect = {x, y, width, height};
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            std::cout << "Button clicked at (" << x << ", " << y << ")" << std::endl;
            onClick();
            return true;
        }
    }
    return false;
}

void Button::onClick(){
}

//constr,destructor
Start::Start(int _x, int _y, const string& image_path) : Button(_x, _y, 300, 120,image_path.c_str()){}
Start::~Start(){}
bool Start::PollEvents(SDL_Event& event){
	return Button::PollEvents(event);
}
void Start::draw(){
	Button::draw();
}

Settings::Settings(int _x, int _y, const string& image_path) : Button(_x, _y, 300, 110, image_path.c_str()){}
Settings::~Settings(){}
bool Settings::PollEvents(SDL_Event& event){
	return Button::PollEvents(event);
}
void Settings::draw(){
	Button::draw();
}

MapChoice::MapChoice(int _x, int _y, const string& image_path) : Button(_x, _y, 150, 90,image_path.c_str()){}
MapChoice::~MapChoice(){}
bool MapChoice::PollEvents(SDL_Event& event){
	return Button::PollEvents(event);
}
void MapChoice::draw(){
	Button::draw();
}

//clicklogic
void Start::onClick(){
	cerr << "愛你喔宏昀" << endl;
}
void Settings::onClick(){
	cerr << "愛你喔宏昀" << endl;
}
void MapChoice::onClick(){
	cerr << "愛你喔宏昀" << endl;
}


demo::demo(int _x, int _y, const string& image_path): Button(_x, _y, 380, 285, image_path.c_str()) {};
demo::demo(int _x, int _y, int _h, int _w, const string& image_path): Button(_x, _y, _h, _w, image_path.c_str()) {};
demo::~demo() {}
void demo::draw(){
    Button::draw();
}



//
// Created by Hans Chiang on 2024/12/21.
//