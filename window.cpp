#include<iostream>
#include<tuple>
#include "Headers/window.h"
#include "Headers/text.h"
#include "Headers/button.h"
#include<graphics.h>
#include<SDL_image.h>
#include <SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>
#define pb push_back

SDL_Renderer *Window::renderer = nullptr;

Window::Window(const string& _title, int _width, int _height)
    : title(_title), width(_width), height(_height), AI(0), restart(1){
    closed = !init();
}

void Window::init2(const string& _title, int _width, int _height){
    title = _title, width = _width, height = _height;
    closed = !init();
}

Window::Window(){

}

Window::~Window(){
	cleanupAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Window::init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        cerr<<"Failed to initialize SDL.\n";
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        cerr << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
        return false;
    }
    if(TTF_Init() == -1){
        cerr<<"Failed to initialize SDL_ttf.\n";
        return false;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
    	cerr << "Failed to initialize SDL_Mixer: " << endl;
    	return false;
	}
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        0
    );
    if(window == nullptr){
        cerr<<"Failed to create window.\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr){
        cerr<<"Failed to create renderer.\n";
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return true;
}

SDL_Rect rect = {300, 300, 100, 100};

void Window::PollEvents(SDL_Event event){
    switch(event.type){
        case SDL_QUIT:
            closed = true;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    closed = true;
                    stopMusic();
                    break;
                case SDLK_BACKSPACE:
                    cout<<"pressed backspace\n";
                    restart = true;
                    break;
                case SDLK_i :
                    AI ^= 1;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            auto [x, y] = tie(event.motion.x, event.motion.y);
            cout<<"Clicked : ("<<x<<", "<<y<<")\n";
            break;
        }
        default:
            break;
    }
}

void Window::clear() const {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 74, 88, 181, 255);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

bool Window::loadMusic(const string& musicPath) {
    backgroundMusic = Mix_LoadMUS(musicPath.c_str());
    if (!backgroundMusic) {
        cerr << "Failed to load music: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}

void Window::playMusic(int loops) {
    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, loops);
    }
}

void Window::stopMusic() {
    Mix_HaltMusic();
}

bool Window::loadSoundEffect(const string& soundPath) {
    soundEffect = Mix_LoadWAV(soundPath.c_str());
    if (!soundEffect) {
        cerr << "Failed to load sound effect: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}

void Window::playSoundEffect() {
    if (soundEffect) {
        Mix_PlayChannel(-1, soundEffect, 0);
    }
}

void Window::cleanupAudio() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
        soundEffect = nullptr;
    }
    Mix_CloseAudio();
}

void Window::displayGameName(){
	bool running = true;
    SDL_Event event;
    string GameName = "It's MyGO!!!!!";
    string Notice = "[ Press Space to skip ]";
    //Text line(renderer, "Resources/arial.ttf", 50, &GameName, {255, 255, 255, 255});
    Text line2(renderer, "Resources/arial.ttf", 20, &Notice, {255, 255, 255, 255});

	int alpha = 0;
    const Uint8 fadeStep = 5;
    const Uint32 holdDuration = 2000;
    Uint32 startTime;
	int last = ct;
    while (alpha < 255) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(event.type == SDL_KEYDOWN){
        		int op = event.key.keysym.sym;
        		if(op == SDLK_SPACE) return;
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect rect{0, 0, 800, 600};SDL_Rect logo{200, 70, 400, 200};
        SDL_Surface* surface1 = IMG_Load("Images/background.png");
        SDL_Surface* surface2 = IMG_Load("Images/Mygo.png");
        SDL_Texture* icon1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Texture* icon2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_SetTextureAlphaMod(icon1,alpha);SDL_SetTextureAlphaMod(icon2,alpha);
        SDL_FreeSurface(surface1);SDL_FreeSurface(surface2);
        SDL_RenderCopy(renderer, icon1, nullptr, &rect);SDL_RenderCopy(renderer, icon2, nullptr, &logo);
        SDL_DestroyTexture(icon1);SDL_DestroyTexture(icon2);
        //line.setAlpha(alpha);
		//line.display(260, 200, renderer);
		line2.setAlpha(alpha);
		line2.display(300, 500, renderer);
        SDL_RenderPresent(renderer);
        alpha = (ct - last) / 10;
    }
	alpha = 255;
    startTime = SDL_GetTicks();
    while (SDL_GetTicks() - startTime < holdDuration) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(event.type == SDL_KEYDOWN){
        		int op = event.key.keysym.sym;
        		if(op == SDLK_SPACE) return;
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect rect{0, 0, 800, 600};SDL_Rect logo{200, 70, 400, 200};
        SDL_Surface* surface1 = IMG_Load("Images/background.png");
        SDL_Surface* surface2 = IMG_Load("Images/Mygo.png");
        SDL_Texture* icon1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Texture* icon2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_SetTextureAlphaMod(icon1,alpha);SDL_SetTextureAlphaMod(icon2,alpha);
        SDL_FreeSurface(surface1);SDL_FreeSurface(surface2);
        SDL_RenderCopy(renderer, icon1, nullptr, &rect);SDL_RenderCopy(renderer, icon2, nullptr, &logo);
        SDL_DestroyTexture(icon1);SDL_DestroyTexture(icon2);

        //line.display(260, 200, renderer);
        line2.display(300, 500, renderer);
        SDL_RenderPresent(renderer);
    }
	alpha = 255;
	last = ct;
    while (alpha > 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(event.type == SDL_KEYDOWN){
        		int op = event.key.keysym.sym;
        		if(op == SDLK_SPACE) return;
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect rect{0, 0, 800, 600};SDL_Rect logo{200, 70, 400, 200};
        SDL_Surface* surface1 = IMG_Load("Images/background.png");
        SDL_Surface* surface2 = IMG_Load("Images/Mygo.png");
        SDL_Texture* icon1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Texture* icon2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_SetTextureAlphaMod(icon1,alpha);SDL_SetTextureAlphaMod(icon2,alpha);
        SDL_FreeSurface(surface1);SDL_FreeSurface(surface2);
        SDL_RenderCopy(renderer, icon1, nullptr, &rect);SDL_RenderCopy(renderer, icon2, nullptr, &logo);
        SDL_DestroyTexture(icon1);SDL_DestroyTexture(icon2);

		//line.setAlpha(alpha);
		//line.display(260, 200, renderer);
		line2.setAlpha(alpha);
		line2.display(300, 500, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
        alpha = 255 - (ct - last) / 12;
    }

}

void Window::displayWelcomeMessage() {
    bool running = true;
    SDL_Event event;

	const vector<vector<Uint16>> Texts = {
    {0x300C, 0x723D, 0x4E16, 0x002E, 0x002E, 0x002E, 0x4F60, 0x70BA, 0x4F55, 0x4E0D, 0x4F86, 0x7DF4, 0x5718, 0x4E86, 0xFF1F, 0x300D, 0x71C8, 0x795E, 0x60C5, 0x932F, 0x6115, 0x7684, 0x8AAA, 0x9053, 0x3002, 0},
    {0x300C, 0x4E0D, 0x8981, 0x518D, 0x4F86, 0x627E, 0x6211, 0x4E86, 0xFF01, 0x70BA, 0x4EC0, 0x9EBC, 0x8981, 0x6F14, 0x594F, 0x6625, 0x65E5, 0x5F71, 0xFF1F, 0x70BA, 0x4EC0, 0x9EBC, 0xFF1F, 0x300D, 0},
    {0x71C8, 0x9ED8, 0x9ED8, 0x5730, 0x4F4E, 0x982D, 0xFF0C, 0x90A3, 0x662F, 0x0043, 0x0072, 0x0079, 0x0063, 0x0068, 0x0069, 0x0063, 0x5927, 0x5BB6, 0x7684, 0x6B4C, 0x3002, 0},
    {0x300C, 0x4F60, 0x660E, 0x77E5, 0x9053, 0x7965, 0x5B50, 0x6703, 0x96E3, 0x904E, 0x7684, 0xFF0C, 0x537B, 0x9084, 0x662F, 0x81EA, 0x9867, 0x81EA, 0x7684, 0x6F14, 0x594F, 0x4E86, 0x3002, 0x300D, 0},
    {0x300C, 0x6EFF, 0x8166, 0x5B50, 0x90FD, 0x60F3, 0x8457, 0x81EA, 0x5DF1, 0x5462, 0x3002, 0x300D, 0x71C8, 0x5FFD, 0x7136, 0x51B7, 0x51B7, 0x7684, 0x8AAA, 0x9053, 0x3002, 0},
    {0x300C, 0x4EC0, 0x9EBC, 0xFF1F, 0x4F60, 0x8AAA, 0x4EC0, 0x9EBC, 0xFF1F, 0x300D, 0x6C92, 0x9069, 0x61C9, 0x7A81, 0x7136, 0x5176, 0x4F86, 0x7684, 0x8F49, 0x8B8A, 0x7684, 0x723D, 0x4E16, 0x3002, 0},
    {0x9019, 0x53E5, 0x8A71, 0x002E, 0x002E, 0x002E, 0x597D, 0x50CF, 0x5728, 0x54EA, 0x88CF, 0x807D, 0x904E, 0x5462, 0x002E, 0x002E, 0x002E, 0x662F, 0x7965, 0x5B50, 0x55CE, 0x002E, 0x002E, 0x002E, 0},
    {0x300C, 0x4E00, 0x8D77, 0x8207, 0x6211, 0x6C7A, 0x9B25, 0x4E00, 0x628A, 0x5427, 0xFF0C, 0x723D, 0x4E16, 0x3002, 0x300D, 0x71C8, 0x5805, 0x5B9A, 0x5730, 0x8AAA, 0x9053, 0x3002, 0},
    {0x300C, 0x5C31, 0x4E00, 0x628A, 0xFF0C, 0x8D0F, 0x4E86, 0x5C31, 0x548C, 0x6211, 0x56DE, 0x53BB, 0x7DF4, 0x8A69, 0x8D85, 0x7D46, 0x5427, 0x300D, 0},
    {0x300C, 0x6211, 0x7B54, 0x61C9, 0x4F60, 0x3002, 0x300D, 0x300C, 0x9084, 0x6709, 0x966A, 0x6211, 0x53BB, 0x0058, 0x0070, 0x0061, 0x0072, 0x006B, 0x770B, 0x0054, 0x006F, 0x006D, 0x006F, 0x0072, 0x0069, 0x006E, 0x3002, 0x300D, 0},
    {0x7A76, 0x7ADF, 0x004D, 0x0079, 0x0067, 0x006F, 0x0021, 0x0021, 0x0021, 0x0021, 0x0021, 0x5718, 0x7684, 0x751F, 0x6B7B, 0x5982, 0x4F55, 0x5462, 0xFF1F, 0x672A, 0x5B8C, 0x5F85, 0x7E8C, 0x002E, 0x002E, 0x002E, 0}
	};

	vector<Text*> lines;

	for(const auto& text : Texts){
		lines.pb(new Text(renderer, "Resources/kaiu.ttf", 20, text.data(), {255, 255, 255, 255}));
	}

	int startX = 160;
	int startY = 30;
	int linespace = 50;

    int alpha = 0;
    const Uint8 fadeStep = 5;
    const Uint32 holdDuration = 10000;
    Uint32 startTime;
	int last = ct;
    while (alpha < 255) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(event.type == SDL_KEYDOWN){
        		int op = event.key.keysym.sym;
        		if(op == SDLK_SPACE) return;
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for(size_t i=0;i<lines.size();i++){
        	lines[i]->setAlpha(alpha);
        	lines[i]->display(startX, startY+i*linespace,renderer);
		}

        SDL_RenderPresent(renderer);
        alpha = (ct - last) / 12;
    }

    startTime = SDL_GetTicks();
    while (SDL_GetTicks() - startTime < holdDuration) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(event.type == SDL_KEYDOWN){
        		int op = event.key.keysym.sym;
        		if(op == SDLK_SPACE) return;
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for(size_t i=0;i<lines.size();i++){
        	lines[i]->display(startX, startY+i*linespace,renderer);
		}

        SDL_RenderPresent(renderer);
    }
    
	last = ct;
    while (alpha > 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for(size_t i=0;i<lines.size();i++){
        	lines[i]->setAlpha(alpha);
        	lines[i]->display(startX, startY+i*linespace,renderer);
		}

        SDL_RenderPresent(renderer);
        alpha = 255 - (ct - last) / 12;
    }

    for (auto line : lines) {
    	delete line;
	}
	lines.clear();
}

void Window::show_button(){
	bool running = true;
    SDL_Event event;
    selectedmap = 0;

    Start startButton(250, 250, "Images/start.png");
    Settings optionButton(250, 400, "Images/setting.png");
	while (running && !closed) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }

            if (startButton.PollEvents(event)) {
                running = false;
            }

            if (optionButton.PollEvents(event)) {
                selection();
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 繪製按鈕

	    SDL_Rect rect{0, 0, 800, 600};
        SDL_Surface* surface = IMG_Load("Images/Band.png");
        SDL_Texture* icon = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, icon, nullptr, &rect);
        SDL_DestroyTexture(icon);
        startButton.draw();
        optionButton.draw();

        SDL_RenderPresent(renderer);
    }
}
void Window::displayStartScreen() {
    displayGameName();
    displayWelcomeMessage();
}
void Window::selection() {
    bool running = true;
    SDL_Event event;
    MapChoice MapChoice1(128, 80, "Images/map1.png");
    MapChoice MapChoice2(522, 80, "Images/map2.png");

    demo map1_demo(13, 200,"Images/map1_demo.png");
    demo map2_demo(407, 200,"Images/map2_demo.png");
    while(running && !closed) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(MapChoice1.PollEvents(event)) {
                selectedmap = 0;
                running = false;
            }
            if(MapChoice2.PollEvents(event)) {
                selectedmap = 1;
                running = false;
            }
        }

        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
        SDL_Rect rect{0, 0, 800, 600};
        SDL_Surface* surface = IMG_Load("Images/background2.png");
        SDL_Texture* icon = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, icon, nullptr, &rect);
        SDL_DestroyTexture(icon);

        MapChoice1.draw(); map1_demo.draw();
        MapChoice2.draw(); map2_demo.draw();
        SDL_RenderPresent(renderer);
    }
    select_player(0);
    select_player(1);
}

string name_list[] = {"soyo", "tomori", "taki", "anon", "rana"};
double porportion[] = {2.86, 2.84, 2.237, 2.249, 3.295};
int player_selection[2] = {0, 1};

void Window::select_player(int id){
	if(closed) return;
	vector<demo*> list;
	vector<Button*> list2;
	for(int i = 0; i < 5; i++){
		string str = "Images/" + name_list[i] + "_character.png";
		list.pb(new demo(40 + i * 150 - (i >= 2 && i <= 3) * 20 + (i == 2 || i == 4) * 5, 90, int(350 / porportion[i]), 350, str));
		string str2 = "Images/" + name_list[i] + ".png";
		list2.pb(new Button(50 + i * 150, 460, 100, 100, str2));
	}
	SDL_Event event;
	while(!closed){
		while (SDL_PollEvent(&event)) {
	        if (event.type == SDL_QUIT) {
	            closed = true;
	            return;
	        }
	        for(int i = 0; i < 5; i++){
	        	if(list2[i]->PollEvents(event)){
	        		player_selection[id] = i;
	        		return;
				}
			}
	    }
	    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    //SDL_RenderClear(renderer);
	    SDL_Rect rect{0, 0, 800, 600};
	    SDL_Surface* surface = IMG_Load("Images/background2.png");
	    SDL_Texture* icon = SDL_CreateTextureFromSurface(renderer, surface);
	    SDL_FreeSurface(surface);
	    SDL_RenderCopy(renderer, icon, nullptr, &rect);
	    SDL_DestroyTexture(icon);

		string str = "Select Player " + string(1, char('1' + id)) + (!id ? " (Left)" : " (Right)");
		Text msg(renderer, "Resources/consola.ttf", 40, &str, {255, 255, 255, 255});
		msg.display(160 - (id * 10), 20, renderer);
		for(int i = 0; i < 5; i++) list[i]->draw();
		for(int i = 0; i < 5; i++) list2[i]->draw();
        SDL_RenderPresent(renderer);
	}
}

void Window::handle_restart(string str){
	restart = 0;
	bool running = true;
	SDL_Event event;
    loadMusic("Resources/詩超絆.mp3");
    Mix_VolumeMusic(35);
    playMusic(-1);
	Button rst(100, 300, 200, 80, "Images/restart.png");
	Button home(500, 300, 200, 80, "Images/home.png");
    while(running && !closed) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                closed = true;
                return;
            }
            if(rst.PollEvents(event)) {
                restart = 2;
                running = false;
            }
            if(home.PollEvents(event)) {
                restart = 1;
				loadMusic("Resources/春日影.mp3");
                playMusic(-1);
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
		SDL_Rect rect = {0, 0, 800, 600};
		SDL_Surface* surface = IMG_Load("Images/end.jpg");
        SDL_Texture* icon = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, icon, nullptr, &rect);
        SDL_DestroyTexture(icon);
        rst.draw();
        home.draw();
        SDL_RenderPresent(renderer);
    }
}