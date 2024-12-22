#pragma once

#include<string>
#include <SDL.h>
#include <graphics.h>
#include <SDL_mixer.h>
#include<time.h>
#define ct SDL_GetTicks()
using namespace std;

extern int player_selection[2];
extern string name_list[];
class Window{

public:
  Window(const string& _title, int _width, int _height);
  Window();
  ~Window();
  inline bool isClosed() const { return closed; };
  void displayStartScreen();
  void displayWelcomeMessage();
  void displayGameName();
  void selection();
  void PollEvents(SDL_Event);
  void clear() const; // put stuff on the screen
  void clear2() const; // put stuff on the screen
  bool init();
  void init2(const string& _title, int _width, int _height);
  int restart;
  int selectedmap;
  bool loadMusic(const string& musicPath);
  void playMusic(int loops = -1);
  void stopMusic();
  bool loadSoundEffect(const string& soundPath);
  void playSoundEffect();
  void cleanupAudio();
  void show_button();
  void handle_restart(string s);
  void select_player(int);
  Mix_Music* backgroundMusic = nullptr;
  Mix_Chunk* soundEffect = nullptr;
	int AI;
	
private:
  string title;
  int width = 1000, height = 750;
  bool closed = false;
  SDL_Window *window = nullptr;

public:
  static SDL_Renderer *renderer;
};