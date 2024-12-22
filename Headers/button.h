#pragma once

#include<SDL_image.h>
#include<string>
using namespace std;

class Button{
public:
    Button(int _x, int _y, int _w, int _h, const string& image_path);
    virtual ~Button();
    virtual void draw();
    virtual bool PollEvents(SDL_Event& event);
    virtual void onClick();
    int x, y;
    int width = 300, height = 100;
    bool isHovered = false;
protected:
    SDL_Texture* icon = nullptr;
};

class Start : public Button{
public:
    Start(int _x, int _y, const string& image_path);
    ~Start();
    void draw() override;
    bool PollEvents(SDL_Event& event) override;
    void onClick() override;
};

class Settings : public Button{
public:
    Settings(int _x, int _y, const string& image_path);
    ~Settings();
    void draw() override;
    bool PollEvents(SDL_Event& event) override;
    void onClick() override;
};

class MapChoice : public Button{
public:
    MapChoice(int _x, int _y, const string& image_path);
    ~MapChoice();
    void draw() override;
    bool PollEvents(SDL_Event& event) override;
    void onClick() override;
};

class demo: public Button {
public:
    demo(int _x, int _y, const string& image_path);
    demo(int _x, int _y, int _h, int _w, const string& image_path);
    ~demo();
    void draw() override;
    bool PollEvents(SDL_Event& event) override{return false;}
    void onClick() override{};
};