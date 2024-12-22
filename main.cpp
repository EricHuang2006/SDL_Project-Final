#include <iostream>
#include "graphics.h"
#include <SDL_image.h>
#include "Headers/window.h"
#include "Headers/player.h"
#include "Headers/text.h"
#include "Headers/ball.h"
#include "Headers/cross.h"
#include "Headers/object.h"
#include "Headers/Maps.h"
#include "Headers/item.h"
#include "Headers/button.h"
#include <SDL_mixer.h>
#include<queue>
using namespace std;
#define pb push_back
#define BACKGROUND_MUSIC_VOLUME 35
typedef pair<int, int> pll;
const int INF = 1e9;

vector<Player*> list;
vector<Cross*> atk;
vector<Obj*> obj_list;
vector<item*> item_list;

void Player::AI(){
    static int rg = 300;
    static int prv = 0;
    rg--;
    if(rg) return;
    rg = 300;
    int attacked = 0;
    for(auto c : atk){
        for(int j = 0; j < c->dx.size(); j++){
            if(Collision3(this, c->dx[j])){
        		attacked = 1;
        		break;
            }
        }
        for(int j = 0; j < c->dy.size(); j++){
            if(Collision3(this, c->dy[j])){
                attacked = 1;
                break;
            }
        }
	}
    if(attacked){
    	int u = (rand() - rand()) % 3, r = (rand() - rand()) % 3;
        if(x == 0) r = 10000;
        if(x > 710) r -= 10000;
        if(y == 0) u = -10000;
        if(y > 610) u = 10000;
        if(x < 400) r++;
        if(y < 300) u--;
        for(auto x : atk){
            for(auto rect : x->dx){
                if(rect.x > this->x) r--;
                else r++;
            }
            for(auto rect : x->dy){
                if(rect.y > this->y) u++;
                else u--;
            }
        }
        int id = r > 0, k = rand() % 2;
        x += dx[id], y += dy[id];
        if(!valid_move(this) || k) x -= dx[id], y -= dy[id];
        id = 2 + (u < 0);
        x += dx[id], y += dy[id];
        if(!valid_move(this) || !k) x -= dx[id], y -= dy[id];
        return;
	}
	if(rand() % 10 == 0){ // random walk
		int id = rand() % 4;
        x += dx[id], y += dy[id];
	    if(!valid_move(this)) x -= dx[id], y -= dy[id];
	    return;
	}
    vector<vector<int>> status(16, vector<int>(12)), dp(16, vector<int>(12, INF));
	for(auto x : obj_list){
		status[x->x / 50][x->y / 50] = 1;
	}
	auto inbound = [&](int x, int y) -> bool{
		return x >= 0 && y >= 0 && x < 16 && y < 12;
	};
	queue<pll> q;
	q.push({list[1]->x / 50, list[1]->y / 50});
	dp[list[1]->x / 50][list[1]->y / 50] = 0;
	while(!q.empty()){
		auto [x, y] = q.front(); q.pop();
		for(int d = 0; d < 4; d++){
			int xx = x + dx[d] / 50, yy = y + dy[d] / 50;
			if(!inbound(xx, yy) || dp[xx][yy] != INF || status[xx][yy]) continue;
			dp[xx][yy] = dp[x][y] + 1;
			q.push({xx, yy});
		}
	}
	int id = 0, dist = INF;
	for(int d = 0; d < 4; d++){
		int xx = this->x / 50 + dx[d] / 50, yy = this->y / 50 + dy[d] / 50;
		if(inbound(xx, yy) && dp[xx][yy] < dist) id = d, dist = dp[xx][yy];
	}
	x += dx[id], y += dy[id];
    if(!valid_move(this)) x -= dx[id], y -= dy[id];
    if(cd <= 0 && rand() % 7 == 0 && dist != INF){
        cd = 600;
        cout<<"AI place a ball on ("<<x<<", "<<y<<")\n";
        put_ball(x, y);
    }
}

vector<SDL_Rect> getx(Cross *c){
    vector<SDL_Rect> res;
	SDL_Rect ret = {c->x, 0, 50, 620};
	int l = 0, r = 620;
	for(int i = 0; i < obj_list.size(); i++){
		if(!obj_list[i]->type) continue;
		SDL_Rect cur = {obj_list[i]->x, obj_list[i]->y, 50, 50};
		if(SDL_HasIntersection(&ret, &cur)){
			if(cur.y < c->y) l = max(l, cur.y + 50);
			else r = min(r, cur.y);
		}
	}
	ret.y = l, ret.h = r - l;
	res.pb(ret);
    return res;
}

vector<SDL_Rect> gety(Cross *c){
    vector<SDL_Rect> res;
	SDL_Rect ret = {0, c->y, 800, 50};
	int l = 0, r = 800;
	for(int i = 0; i < obj_list.size(); i++){
		if(!obj_list[i]->type) continue;
		SDL_Rect cur = {obj_list[i]->x, obj_list[i]->y, 50, 50};
		if(SDL_HasIntersection(&ret, &cur)){
			if(cur.x < c->x) l = max(l, cur.x + 50);
			else r = min(r, cur.x);
		}
	}
	ret.x = l, ret.w = r - l;
	res.pb(ret);
    return res;
}

void Cross::draw(){
    dx = getx(this), dy = gety(this);
    if(icon){
        for(auto x : dx){
            SDL_RenderCopy(Window::renderer, icon, nullptr, &x);
        }
        for(auto x : dy){
            SDL_RenderCopy(Window::renderer, icon, nullptr, &x);
        }
    }
    else{
        SDL_SetRenderDrawColor(Window::renderer, 0, 255, 255, 100);
        for(auto x : dx){
            SDL_RenderFillRect(Window::renderer, &x);
        }
        for(auto x : dy){
            SDL_RenderFillRect(Window::renderer, &x);
        }
    }
}

void deal_damage(Cross *c){
    for(int i = 0; i < obj_list.size(); i++){
        if(obj_list[i]->type == 1) continue;
        SDL_Rect cur = {obj_list[i]->x, obj_list[i]->y, 50, 50};
        int ok = 0;
        for(int j = 0; j < c->dx.size(); j++){
            if(SDL_HasIntersection(&c->dx[j], &cur)){
                ok = 1;
                break;
            }
        }
        for(int j = 0; j < c->dy.size(); j++){
            if(SDL_HasIntersection(&c->dy[j], &cur)){
                ok = 1;
                break;
            }
        }
        if(ok) obj_list.erase(obj_list.begin() + i), i--;
    }
    for(int i = 0; i < list.size(); i++){
        int tmp = 0;
        for(int j = 0; j < c->dx.size(); j++){
            if(Collision3(list[i], c->dx[j])){
                if(list[i]->have_shield){list[i]->use_shield = true; break;}
                else{tmp = 1;break;}
            }
        }
        for(int j = 0; j < c->dy.size(); j++){
            if(Collision3(list[i], c->dy[j])){
                if(list[i]->have_shield){list[i]->use_shield = true; break;}
                else{tmp = 1;break;}
            }
        }
        list[i]->health -= tmp;
    }
}

void PollEvents(Window &window, vector<Player*> &a){
    SDL_Event event;
    SDL_PollEvent(&event);
    window.PollEvents(event);
    for(auto x : a){
        x->PollEvents(event);
    }
}

void get_item(Player *a) {
    int s=-1;
    for(int i=0;i<item_list.size();i++){
        if(Collision4(a, item_list[i])) s=i;
    }
    if(s==-1) return;
    else {
        switch(item_list[s]->type) {
            case 0:
            	if(a->health<3000) {
            		if(a->health+item_list[s]->heal()>=3000) a->health=3000;
            		else a->health+=item_list[s]->heal();
            		item_list.erase(item_list.begin() + s);
            	}
				return;
            case 1:
            	if(!a->have_shield) {
            		a->have_shield=true; a->use_shield=false; a->scd = item_list[s]->get_shield_cd();
            		item_list.erase(item_list.begin() + s);
            	}
				return;
            default:
                return;
        }

    }
}
bool valid_move(Player *a){
    if(a->x < 0 || a->y < 0 || a->x + a->w > 800 || a->y + a->h > 620) return false;
    if(Collision(list[0], list[1])) return false;
    for(auto p : obj_list){
        if(Collision2(a, p)) return false;
    }
    return true;
}

void init_box(vector<vector<int>>& current_map){
    for (int i = 0; i < current_map.size(); i++) {
        for (int j = 0; j < current_map[i].size(); j++) {
            if (current_map[i][j] == 1) {
                obj_list.push_back(new Obj(j * 50, i * 50, rand() % 2));
            }
            else if(i || (j >= 6 && j <= 9)){
                if(rand()%7 == 0) {
                	int u = rand() % 3;
                    if(u == 0) item_list.push_back(new healing_potion(j * 50, i * 50));
                    else if(u == 1) item_list.push_back(new shield(j * 50, i * 50));
                }
            }
        }
    }
}
void generate_item(){
	static int gen_time = 5000;
	if(gen_time){
		gen_time--;
		return;
	}
	gen_time = rand() % 2000 + 5000;
	vector<vector<int>> status(12, vector<int>(16));
	int cnt = 12 * 16 - 12;
	for(auto x : obj_list){
		status[x->y / 50][x->x / 50] = 1, cnt--;
	}
	for(auto x : item_list){
		status[x->y / 50][x->x / 50] = 1, cnt--;
	}
	cnt = rand() % cnt;
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < 16; j++){
			if(!i && (j < 6 || j > 9)) continue;
			cnt--;
			if(!status[i][j] && !cnt) {
		    	int u = rand() % 3;
		        if(u == 0 || u == 2) item_list.push_back(new healing_potion(j * 50, i * 50));
		        else if(u == 1) item_list.push_back(new shield(j * 50, i * 50));
		    }
		}
	}
	gen_time--;
}

void draw_background(){
	SDL_SetRenderDrawColor(Window::renderer, 0, 255, 127, 100);
	// SDL_SetRenderDrawColor(Window::renderer, 0, 255, 255, 100);
	for(int i = 50; i < 800; i += 50){
		SDL_Rect cur = {i - 1, 0, 2, 600};
		SDL_RenderFillRect(Window::renderer, &cur);
	}
	for(int i = 50; i < 600; i += 50){
		SDL_Rect cur = {0, i - 1, 800, 2};
		SDL_RenderFillRect(Window::renderer, &cur);
	}
}

#define soyorin Player(100, 100, 50, 50, a, "Images/soyo.png", "Resources/放置音效.mp3")
#define tomorin Player(600, 100, 50, 50, b, "Images/tomori.png", "Resources/放置音效.mp3")
#define p1 Player(100, 100, 50, 50, a, "Images/" + name_list[player_selection[0]] + ".png", "Resources/放置音效.mp3")
#define p2 Player(600, 100, 50, 50, b, "Images/" + name_list[player_selection[1]] + ".png", "Resources/放置音效.mp3")
int endgame = 0;
vector<int> a = {SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_f, SDLK_q};
vector<int> b = {SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_RCTRL, SDLK_RSHIFT};

void newgame(Window& window, vector<vector<int>>& current_map){
	list.clear(), atk.clear(), obj_list.clear(), ball_list.clear(), item_list.clear();
	list.pb(new p1), list.pb(new p2);
	init_box(current_map);
	endgame = 0;
	while(!window.isClosed() && !endgame){
	draw_background();
	generate_item();
	if(window.AI) list[0]->AI();
       	PollEvents(window, list);
        for(auto &x : list){
            x->cd--;
            x->draw();
        }
        for(int i = 0; i < ball_list.size(); i++){
            ball_list[i]->cd--;
            if(ball_list[i]->cd == 0){
                cout<<"explode\n";
                window.loadSoundEffect("Resources/WaterSplash.mp3");
				Mix_VolumeChunk(window.soundEffect, 80);
                window.playSoundEffect();
                atk.pb(new Cross(ball_list[i]->x, ball_list[i]->y));
                ball_list.erase(ball_list.begin() + i);
                i--;
            }
            else ball_list[i]->draw();
        }
        for(int i = 0; i < atk.size(); i++){
            atk[i]->cd--;
            if(atk[i]->cd == 0){
                atk.erase(atk.begin() + i);
                i--;
                continue;
            }
            atk[i]->draw();
            deal_damage(atk[i]);
        }
	    for(int i=0; i<list.size(); i++) {
	        if(list[i]->use_shield) {
	            list[i]->scd--;
	            if(!list[i]->scd){ list[i]->have_shield=false; list[i]->use_shield=false;}
	        }
	    }
	    for(int i = 0; i < list.size(); i++) {
	        get_item(list[i]);
	    }
        for(int i = 0; i < obj_list.size(); i++){
            obj_list[i]->draw();
        }
	    for(int i = 0; i < item_list.size(); i++){
	        item_list[i]->draw();
	    }
        if(list[0]->health <= 0) endgame = 1;
        if(list[1]->health <= 0) endgame = 2;
        if(!endgame) window.clear();
    }
}


int main(int argc, char** argv){
    srand(time(0));
    Window window("MyGO!!!!!", 800, 600);
    initialize_maps();

    if (!window.loadMusic("Resources/春日影.mp3")) {
	    cerr << "Failed to load music. Exiting..." << endl;
        return -1;
    }
    Mix_VolumeMusic(BACKGROUND_MUSIC_VOLUME);

    window.playMusic(-1);

    window.displayStartScreen();
    // wstring tmp = L"你是來找我吵架的嗎?";
    // const Uint16 tmp[] = {20320,26159,20358,25214,25105,21557,26550,30340,21966,63, 0};
    // Text text(Window::renderer, "Resources/kaiu.ttf", 25, tmp, {255, 0, 0, 255});
	while(!window.isClosed()){
		if(window.restart == 1) window.show_button();
	    current_map_index = window.selectedmap;
	    vector<vector<int>>& current_map = get_current_map();
		window.loadMusic("Resources/Ave Mujica.mp3");
		window.playMusic(-1);
		newgame(window, current_map);
		if(window.isClosed()) break;
		// game ends
		SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 200);
		SDL_Rect sp = {0, 0, 800, 600};
		SDL_RenderFillRect(Window::renderer, &sp);
		string str = string("Match end, Player ") + string(1, char('0' + 3 - endgame)) + string(" won the game!");
		cout<<str<<"\n";
		Text end_msg(Window::renderer, "Resources/consola.ttf", 40, &str, {255, 255, 255, 255});
		end_msg.display(45, 150, Window::renderer);
		window.clear();
		window.stopMusic();
		SDL_Delay(3000);
		window.handle_restart(str);
		if(!window.restart) break;
	}
    return 0;
}