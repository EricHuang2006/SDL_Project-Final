#pragma once

#include<vector>
using namespace std;

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 6;


extern vector<vector<vector<int>>> all_maps;
extern int current_map_index;

void initialize_maps();

vector<vector<int>>& get_current_map();	