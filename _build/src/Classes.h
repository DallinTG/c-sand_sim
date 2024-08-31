#pragma once
#include <array>
#include <ranges>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <future>
#include <thread>
#include <omp.h>
#include "raylib.h"

extern int chunck_width;
extern int chunck_height;
extern int world_width;
extern int world_height;
extern int box_scale;
extern int max_index;

extern int frame_id;


extern int src_wdth;
extern int src_hight;

extern bool game_pas;


void add_sand(Camera2D* camera);



class X_Y{
public:
	X_Y(int n_x,int n_y) {
		x = n_x;
		y = n_y;
	}
	int x = 0;
	int y = 0;

};

// Data is the base class
class Data {
public:
	std::string name = "defalt";
	int last_frame_id = 0;
	bool flammable = false;
	bool can_dissolve = false;
	bool can_steam = false;
	bool can_stone = false;
	bool hot = false;
	bool acidic = false;
	bool gas = false;
	bool is_falling = false;
	bool can_fall_through = false;
	bool can_slide = true;
	bool can_flow = false;
	bool indestructible = false;
	bool can_move = true;
	float flammableChance = 100.0;
	bool do_decay = false;
	int decay_time = 100;
	float grav = 0;
	float mass = 100.0;
	float startingTemp = 100.0;
	Color color = {255,255,255,255};
	int color_variance = 20;
	Data(){
	}
	void do_color_variance() {
		int v1 = rand() % color_variance - color_variance / 2;
		color.r = std::clamp((color.r + v1), 0, 255);
		color.g = std::clamp((color.g + v1), 0, 255);
		color.b = std::clamp((color.b + v1), 0, 255);
	}
};

// Solid, Liquid, Gas are the sub class
class Solid :public Data {
	public:


};
class Liquid :public Data {

	public:


		Liquid() {
			can_fall_through = true;

		}
	
};
class Gas :public Data {
	public:
		Gas() {
			can_fall_through = true;
			can_flow = true;
		     mass = 5.0;
			 gas = true;
		}
		
};
// final class type's
class Air :public Gas {
public:
	Air() {
		name = "Air";
		color = { 200,200,200,255 };
		mass = 2.0;
		//do_color_variance();
		
		
	}
};
class Fire :public Gas {
public:
	Fire() {
		name = "Fire";
		color = { 250,167,74,255 };
		color_variance = 40;
		mass = 6.0;
		do_color_variance();
		hot = true;
		int decay_time = rand() % 700 + 300;
		do_decay = true;


	}
};
class Steam :public Gas {
public:
	Steam() {
		name = "Steam";
		color = { 200,200,233,255 };
		color_variance = 40;
		mass = 5.0;
		do_color_variance();
		hot = false;
		int decay_time = rand() % 700 + 300;
		do_decay = true;


	}
};


class Sand :public Solid {
public:
	Sand() {
		name = "Sand";
		color = { 255,215,0,255 };
		do_color_variance();
		mass = 150.0;
		grav = 1;
		can_dissolve = true;
	}
};
class Gravel :public Solid {
public:
	Gravel() {
		name = "Gravel";
		color = { 52,54,58,255 };
		do_color_variance();
		mass = 250.0;
		grav = 1;
	}
};
class Stone :public Solid {
public:
	Stone() {
		name = "Stone";
		color = { 115,120,110,255 };
		do_color_variance();
		mass = 300.0;
		grav = 0;
	}
};

class Wood :public Solid {
public:
	Wood() {
		name = "Wood";
		color = { 55,41,11,255 };
		do_color_variance();
		mass = 100.0;
		flammable = true;
		grav = 0;
		can_dissolve = true;
	}
};

class Watter :public Liquid {
public:
	Watter() {
		name = "Watter";
		color = { 10,40,255,255 };
		do_color_variance();
		grav = 1;
		mass = 50.0;
		can_flow = true;
		can_steam = true;
	}
};

class Acid :public Liquid {
public:
	Acid() {
		name = "Acid";
		color = { 10,245,10,255 };
		do_color_variance();
		grav = 1;
		mass = 75.0;
		can_flow = true;
		flammable = true;
		acidic = true;
	}
};

class Oil :public Liquid {
public:
	Oil() {
		name = "Oil";
		color = { 23,15,0,255 };
		do_color_variance();
		grav = 1;
		mass = 45.0;
		can_flow = true;
		flammable = true;
	}
};

class Lava :public Liquid {
public:
	Lava() {
		name = "Lava";
		color = { 247,158,15,255 };
		do_color_variance();
		grav = 1;
		mass = 240.0;
		hot = true;
		can_flow = true;
		can_stone = true;
	}
};

class Bounding_Box :public Data{
public:
	Bounding_Box() {
		name = "nul";
		color = { 255,215,0,255 };
		do_color_variance();
		float grav = 0;
		bool flammable = false;
		bool can_fall_through = false;
		bool indestructible = true;
		bool can_move = false;
		float mass = 10000.0;
	}

};




//rendering
class Cell {
public:
	Cell() {
		Air air;
		data = (Data)air;
		
		
	}

	int index=0;
	int x=0;
	int y=0;
	int w_x = 0;
	int w_y = 0;
	int chunck_x = 0;
	int chunck_y = 0;
	int chunck_i = 0;
	Data data;

	void draw_cell() {
		Rectangle rec = { x * box_scale,y * box_scale,box_scale, box_scale };
		DrawRectangleRec(rec, data.color);
	}
};






class Chunck {
public:
	Chunck() {
		std::cout << "adas"<< "\n";
		width = chunck_width;
		height = chunck_height;
		max_cells = width * height;

		x = 0;
		y = 0;
		
		cells.resize(height, std::vector<Cell>(width));
	}
	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;
	int max_cells = 0;
	int is_active = 0;
	Cell bounding_box;
	//RenderTexture2D texture_2d;
	std::vector<std::vector<Cell>> cells;
	
};



class World {
public:
	World(int n_width, int n_height) {
		width = n_width;
		height = n_height;

		w_width= chunck_width* n_width;
		w_height = chunck_height * n_height;

		bounding_box.data = bounding_box_data;

		max_chuncks = n_width * n_height;
		chuncks.resize(height, std::vector<Chunck>(width));
		{
			int x = 0;
			for (auto& row : chuncks)
			{
				int y = 0;
				for (auto& chunck : row)
				{
					chunck.x = x;
					chunck.y = y;
					int cell_x = 0;
					for (auto& row_2 : chunck.cells)
					{
						int cell_y = 0;
						for (auto& cell : row_2)
						{
							cell.chunck_x = x;
							cell.chunck_y = y;
							cell.x = cell_x;
							cell.y = cell_y;
							cell.w_x = cell_x + (x * chunck_width);
							cell.w_y = cell_y + (y * chunck_height);
							cell_y++;
						}
						cell_x++;
					}
					y++;
				}
				x++;
			}
		}
		
	}


	Bounding_Box bounding_box_data;
	Cell bounding_box;
	

	int width=0;
	int height=0;
	int w_width=0;
	int w_height=0;
	int max_chuncks=0;
	RenderTexture2D texture_2d;
	std::vector<std::vector<Chunck>> chuncks;
//_____________________________________________________________________________________________________________

	void render_full() {
		BeginTextureMode(texture_2d);
		for (auto& row : chuncks)
		{
			for (auto& chunck : row)
			{
				
				for (auto& row : chunck.cells)
				{
					for (auto& cell : row)
					{
						Rectangle rec = { cell.w_x * box_scale,cell.w_y * box_scale,box_scale, box_scale };

						DrawRectangleRec(rec, cell.data.color);
					}
				}
			}
		}
		EndTextureMode();
	}

	void full_reset() {
		Air air;
		for (auto& row : chuncks)
		{
			for (auto& chunck : row)
			{

				for (auto& row : chunck.cells)
				{
					for (auto& cell : row)
					{
						cell.data = air;
					}
				}
			}
		}
	}
	
	void draw_world() {
		DrawRectangle(-10, -10, (float)texture_2d.texture.width + 20, (float)texture_2d.texture.height + 20, { 100,100,100,255 });
		DrawTextureRec(texture_2d.texture, Rectangle{ 0,0, (float)texture_2d.texture.width, (float)-texture_2d.texture.height }, Vector2{ 0, 0}, WHITE);
	}

//____________________________________________________________________________________________

	void do_physics(Camera2D* camera) {
		BeginTextureMode(texture_2d);
		add_sand(camera);
		if (!game_pas)
		{
			for (auto& row : chuncks)
			{
				for (auto& chunck : row)
				{



					if (chunck.is_active > 0)
					{
						//std::async(std::launch::async, [&]() {
						chunck.is_active--;

						{

							for (auto& row : chunck.cells)
							{
								for (auto& cell : row)
								{
									physics(cell);
								}
							}
						}
						//});
					}

				}
			}
		}
		EndTextureMode();
	}
	void physics(Cell &cell) {
		if (cell.data.last_frame_id != frame_id)
		{
			Cell* cell_under = get_cell_by_w_xy(cell.w_x, cell.w_y + cell.data.grav);
			Cell* cell_up = get_cell_by_w_xy(cell.w_x, cell.w_y - cell.data.grav);
			Cell* cell_left = get_cell_by_w_xy(cell.w_x - 1, cell.w_y + cell.data.grav);
			Cell* cell_right = get_cell_by_w_xy(cell.w_x + 1, cell.w_y + cell.data.grav);
			Cell* cell_left_left = get_cell_by_w_xy(cell.w_x - 1, cell.w_y);
			Cell* cell_right_right = get_cell_by_w_xy(cell.w_x + 1, cell.w_y);
			Cell* cell_left_up = get_cell_by_w_xy(cell.w_x - 1, cell.w_y - cell.data.grav);
			Cell* cell_right_up = get_cell_by_w_xy(cell.w_x + 1, cell.w_y - cell.data.grav);
			if (cell.data.grav == 0)
			{
				cell_under = get_cell_by_w_xy(cell.w_x, cell.w_y + 1);
				cell_up = get_cell_by_w_xy(cell.w_x, cell.w_y - 1);
				cell_left_up = get_cell_by_w_xy(cell.w_x - 1, cell.w_y - 1);
				cell_right_up = get_cell_by_w_xy(cell.w_x + 1, cell.w_y - 1);
				cell_left = get_cell_by_w_xy(cell.w_x - 1, cell.w_y + 1);
				cell_right = get_cell_by_w_xy(cell.w_x + 1, cell.w_y + 1);
			}
			if (cell.data.flammable)
			{
				do_flammable(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
			}
			if (cell.data.can_dissolve)
			{
				do_disolve(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
			}
			if (cell.data.can_steam)
			{
				do_steame(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
			}

			if (cell.data.gas)
			{
				if (cell.data.can_flow && cell_right_right->data.can_fall_through && cell_right_right->data.mass < cell.data.mass)
				{
					do_flow(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
				else if (cell.data.can_flow && cell_left_left->data.can_fall_through && cell_left_left->data.mass < cell.data.mass)
				{
					do_flow(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
				if ((rand() % 2) == 0)
				{
					if (cell_under->data.can_fall_through && cell_under->data.mass < cell.data.mass)
					{
						swop_data(&cell, cell_under);
						physics(*cell_up);
					}
				}
				else
				{
					if (cell_up->data.can_fall_through && cell_up->data.mass < cell.data.mass)
					{
						swop_data(&cell, cell_up);
						physics(*cell_under);
					}
				}
			}
		
			else if (cell.data.grav != 0)
			{		
			
				if (cell_under->data.can_fall_through && cell_under->data.mass < cell.data.mass)
				{
					
					swop_data(&cell, cell_under);
					physics(*cell_up);
				}
				else if (cell.data.can_flow && cell_right_right->data.can_fall_through && cell_right_right->data.mass < cell.data.mass)
				{
					do_flow(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
				else if (cell.data.can_flow && cell_left_left->data.can_fall_through && cell_left_left->data.mass < cell.data.mass)
				{
					do_flow(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
				else if (cell_left->data.can_fall_through && cell_left->data.mass < cell.data.mass)
				{
					do_slide(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
				else if (cell_right->data.can_fall_through && cell_right->data.mass < cell.data.mass)
				{
					do_slide(&cell, cell_left, cell_right, cell_up, cell_left_up, cell_right_up, cell_left_left, cell_right_right, cell_under);
				}
			}
		}
		if (cell.data.do_decay)
		{
			Air air;
			cell.data.decay_time--;
			if (cell.data.decay_time < 0)
			{
				cell.data = air;
				draw_cell(&cell);
			}
		}
	}


	void do_slide(Cell* cell, Cell* cell_left, Cell* cell_right, Cell* cell_up , Cell* cell_left_up, Cell* cell_right_up, Cell* cell_left_left, Cell* cell_right_right, Cell* cell_under) {
		
		if ((rand() % 2) == 0)
		{
			if (cell_left->data.can_fall_through && cell_left->data.mass < cell->data.mass)
				{;
					swop_data(cell, cell_left);
					physics(*cell_right_up);
				}
			else //if (cell_right->data.can_fall_through && cell_right->data.mass < cell->data.mass)
				{
					swop_data(cell, cell_right);
					physics(*cell_left_up);
					
				}
			}
		else 
		{
		if (cell_right->data.can_fall_through && cell_right->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_right);
				physics(*cell_left_up);
			}
			else //if (cell_left->data.can_fall_through && cell_left->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_left);
				physics(*cell_right_up);
			}
		}
	}
	void do_flow(Cell* cell, Cell* cell_left, Cell* cell_right, Cell* cell_up, Cell* cell_left_up, Cell* cell_right_up, Cell* cell_left_left, Cell* cell_right_right, Cell* cell_under) {
		if ((rand() % 2) == 0)
		{
			if (cell_left_left->data.can_fall_through && cell_left_left->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_left_left);
				physics(*cell_right_right);
			}
			else if (cell_right_right->data.can_fall_through && cell_right_right->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_right_right);
				physics(*cell_left_left);
			}
		}
		else
		{
			if (cell_right_right->data.can_fall_through && cell_right_right->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_right_right);
				physics(*cell_left_left);
			}
			else if (cell_left_left->data.can_fall_through && cell_left_left->data.mass < cell->data.mass)
			{
				swop_data(cell, cell_left_left);
				physics(*cell_right_right);
			}
		}
	}
	void do_flammable(Cell* cell, Cell* cell_left, Cell* cell_right, Cell* cell_up, Cell* cell_left_up, Cell* cell_right_up, Cell* cell_left_left, Cell* cell_right_right, Cell* cell_under) {
		Fire fire;
		if (cell_up->data.hot && cell_up->data.last_frame_id == frame_id)
		{
			cell->data = fire;
			draw_cell(cell);
		}else if (cell_under->data.hot && cell_under->data.last_frame_id == frame_id)
		{
			cell->data = fire;
			draw_cell(cell);
		}
		else if (cell_left->data.hot && cell_left->data.last_frame_id == frame_id)
		{
			cell->data = fire;
			draw_cell(cell);
		}
		else if (cell_right->data.hot && cell_right->data.last_frame_id == frame_id)
		{
			cell->data = fire;
			draw_cell(cell);
		}
	}
	void do_disolve(Cell* cell, Cell* cell_left, Cell* cell_right, Cell* cell_up, Cell* cell_left_up, Cell* cell_right_up, Cell* cell_left_left, Cell* cell_right_right, Cell* cell_under) {
		Air air;
		if (cell_up->data.acidic && cell_up->data.last_frame_id == frame_id)
		{
			cell->data = air;
			draw_cell(cell);
		}
		else if (cell_under->data.acidic && cell_under->data.last_frame_id == frame_id)
		{
			cell->data = air;
			draw_cell(cell);
		}
		else if (cell_left->data.acidic && cell_left->data.last_frame_id == frame_id)
		{
			cell->data = air;
			draw_cell(cell);
		}
		else if (cell_right->data.acidic && cell_right->data.last_frame_id == frame_id)
		{
			cell->data = air;
			draw_cell(cell);
		}
	}
	void do_steame(Cell* cell, Cell* cell_left, Cell* cell_right, Cell* cell_up, Cell* cell_left_up, Cell* cell_right_up, Cell* cell_left_left, Cell* cell_right_right, Cell* cell_under) {
		Steam steame;
		Stone stone;
		if (cell_up->data.hot && cell_up->data.last_frame_id == frame_id)
		{
			cell->data = steame;
			if (cell_up->data.can_stone)
			{
				cell_up->data = stone;
			}
		
			draw_cell(cell);
			draw_cell(cell_up);
		}
		else if (cell_under->data.hot && cell_under->data.last_frame_id == frame_id)
		{
			cell->data = steame;
			if (cell_under->data.can_stone)
			{
				cell_under->data = stone;
			}

			draw_cell(cell);
			draw_cell(cell_up);
		}
		else if (cell_left->data.hot && cell_left->data.last_frame_id == frame_id)
		{
			cell->data = steame;
			if (cell_left->data.can_stone)
			{
				cell_left->data = stone;
			}

			draw_cell(cell);
			draw_cell(cell_up);
		}
		else if (cell_right->data.hot && cell_right->data.last_frame_id == frame_id)
		{
			cell->data = steame;
			if (cell_right->data.can_stone)
			{
				cell_right->data = stone;
			}

			draw_cell(cell);
			draw_cell(cell_up);
		}
	}

	Cell* get_cell_by_w_xy(int w_x, int w_y) {
		if (w_x > 0 && w_y >-1&& w_x < w_width-1 && w_y < w_height-1 )
		{
			int c_x = w_x / chunck_width;
			int c_y = w_y / chunck_height;
			int l_x = w_x % (chunck_width);
			int l_y = w_y % (chunck_height);

			return &chuncks[c_x][c_y].cells[l_x][l_y];
		}
		else
		{
			return &bounding_box;
	
		}
	}
	Chunck* get_chunck_by_w_xy(int w_x, int w_y) {
		int c_x = w_x / chunck_width;
		int c_y = w_y / chunck_height;
		return &chuncks[c_x][c_y];
	}

	void swop_data(Cell* cell_1, Cell* cell_2) {
		if (cell_1->data.can_move && cell_2->data.can_move) {
			Data temp_data = cell_1->data;
			cell_1->data = cell_2->data;
			cell_2->data = temp_data;
			cell_1->data.last_frame_id = frame_id;
			cell_2->data.last_frame_id = frame_id;
			draw_cell(cell_1);
			draw_cell(cell_2);
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y+1)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y-1)->is_active = 3;

			get_chunck_by_w_xy(cell_1->w_x-1, cell_1->w_y)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x+1, cell_1->w_y)->is_active = 3;

			get_chunck_by_w_xy(cell_2->w_x, cell_2->w_y)->is_active = 3;
			//std::cout << cell_1->x << "," << cell_2->y << "|" << cell_1->x << "," << cell_1->y << "\n";

		}
	}
	void swop_data_(Cell* cell_1, Cell* cell_2) {
		if (cell_1->data.can_move && cell_2->data.can_move) {
			Data temp_data = cell_1->data;
			cell_1->data = cell_2->data;
			cell_2->data = temp_data;
			draw_cell(cell_1);
			draw_cell(cell_2);
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y + 1)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x, cell_1->w_y - 1)->is_active = 3;

			get_chunck_by_w_xy(cell_1->w_x - 1, cell_1->w_y)->is_active = 3;
			get_chunck_by_w_xy(cell_1->w_x + 1, cell_1->w_y)->is_active = 3;

			get_chunck_by_w_xy(cell_2->w_x, cell_2->w_y)->is_active = 3;
			//std::cout << cell_1->x << "," << cell_2->y << "|" << cell_1->x << "," << cell_1->y << "\n";

		}
	}

	void set_cell(Vector2& xy, Data& n_data) {
		if (xy.x > 0 && xy.y > -1 && xy.x < w_width - 1 && xy.y < w_height-1) {
			Cell* cell = get_cell_by_w_xy(xy.x, xy.y);
			if (!cell->data.indestructible)
			{
				cell->data = n_data;
				draw_cell(cell);
				get_chunck_by_w_xy(cell->w_x, cell->w_y)->is_active = 3;
				//std::cout << xy.x << "," << xy.y << "|" << cell->x << "," << cell->y << "|" << cell->w_x << "," << cell->w_y << "\n";
			}
		
		}
	}

	void draw_cell(Cell* c_cell) {
		Rectangle rec = { c_cell->w_x * box_scale,c_cell->w_y * box_scale,box_scale, box_scale };
		DrawRectangleRec(rec, c_cell->data.color);
	}


};
