#include "Classes.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

int brush_size = 10;
int material_selector = 1;
int world_width = 10;
int world_height = 10;
int chunck_width = 48;
int chunck_height = 48;
int box_scale = 1;
int max_index = chunck_width * chunck_height;
int frame_id = 0;
bool game_pas = false;
bool space = false;

Bounding_Box bounding_box;


int src_wdth = 800 
//chunck_width * box_scale
;
int src_hight = 800
//chunck_height * box_scale
;

Color bg_color = {20 ,20 ,35 ,255 };
Color test_color = {0 ,0 ,0 ,255 };

World world(world_width, world_height);



const int mats_amount = 10;
Watter watter;
Sand sand;
Stone stone;
Wood wood;
Air air;
Oil oil;
Gravel gravel;
Lava lava;
Fire fire;
Acid acid;
Data mats_list[mats_amount] {watter, sand,gravel,stone ,wood, lava,fire,oil, acid, air};
Data Curent_mats = sand;



 void add_sand(Camera2D* camera) {
	Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *camera);
	mouse_position.x = (int)((mouse_position.x - (brush_size / 2)) / box_scale);
	mouse_position.y = (int)((mouse_position.y - (brush_size / 2)) / box_scale);
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		//Sand sand;
		//world.set_cell(mouse_position, sand);
		//Data mat = Curent_mats;
		
	 	for (int i = 0; i < brush_size; i++) {
	 		mouse_position.x = (int)((mouse_position.x) + (box_scale));
	 		for (int i2 = 0; i2 < brush_size; i2++) {
	 			mouse_position.y = (int)((mouse_position.y) + (box_scale));
	 			//Watter sand_2;
				Data mat = Curent_mats;
				if (mat.name != "Air")
				{
					mat.do_color_variance();
				}
	 			world.set_cell(mouse_position, mat);
	 		}
			mouse_position.y = (int)((GetScreenToWorld2D(GetMousePosition(), *camera).y - (brush_size / 2)) / box_scale);
	 	}


	 	Vector2 p = mouse_position;
	 }
}

void zoom_move_cam(Camera2D* camera) {
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / camera->zoom);
		camera->target = Vector2Add(camera->target, delta);
	}
	float wheel = GetMouseWheelMove();
	if (wheel != 0)
	{
		// Get the world point that is under the mouse
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

		// Set the offset to where the mouse is
		camera->offset = GetMousePosition();

		// Set the target to match, so that the camera maps the world space point 
		// under the cursor to the screen space point under the cursor at any zoom
		camera->target = mouseWorldPos;

		// Zoom increment
		const float zoomIncrement = 0.125f;

		camera->zoom += (wheel * zoomIncrement);
		if (camera->zoom < zoomIncrement) camera->zoom = zoomIncrement;
	}
}

void change_brush_size() {
if(IsKeyDown(KEY_UP) && brush_size <= 50){
		brush_size += 1;
	}
if(IsKeyDown(KEY_DOWN)&& brush_size >= 0){
		brush_size -= 1;
	}
}

void change_material_selector() {
	material_selector += abs(GetMouseWheelMove());
	material_selector = abs(material_selector);
	material_selector % mats_amount;
	Curent_mats = mats_list[material_selector % mats_amount];
}

void check_inputs(Camera2D* camera) {
	if(IsKeyDown(KEY_LEFT_SHIFT) != true){
	zoom_move_cam(camera);
	}
	if(IsKeyDown(KEY_LEFT_SHIFT)){
		change_material_selector();
	}
	change_brush_size();
	//game_pas = IsKeyDown(KEY_SPACE);
	space = IsKeyPressed(KEY_SPACE);
	if (game_pas)
	{
		if (space)
		{
			game_pas = false;
		}
	}
	else
	{
		if (space)
		{
			game_pas = true;
		}
	}
	if (IsKeyPressed(KEY_TAB))
	{
		world.full_reset();
		world.render_full();
	}
	
	

}

int main() {
	
	

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);    // Window configuration flags
	InitWindow(src_wdth, src_hight, "test");
	SetTargetFPS(60);
	Camera2D camera = { 0 };
	camera.zoom = 1.5f;
	camera.offset.x = camera.offset.x + 40;
	camera.offset.y = camera.offset.y + 40;
	world.texture_2d = LoadRenderTexture(chunck_width * box_scale * world_width, chunck_height * box_scale * world_height);

	world.render_full();
	
	BeginDrawing();
	ClearBackground(bg_color);
	EndDrawing();
	while (WindowShouldClose() == false) {
		frame_id++;
		if (frame_id > 100000)
		{
			frame_id = 0;
		}
		//inputs________________________________________________________

		check_inputs(&camera);
		//_________________________________________________

			world.do_physics(&camera);
		
		//world.render_full();

		//Rendering_____________________________________________________
		BeginDrawing();
		BeginMode2D(camera);

			ClearBackground(bg_color);
			world.draw_world();

			Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);
			mouse_position.x = (int)((mouse_position.x - (brush_size / 2)) / box_scale) +1;
			mouse_position.y = (int)((mouse_position.y - (brush_size / 2)) / box_scale) +1;

			//DrawRectangle(mouse_position.x, mouse_position.y, brush_size, brush_size, { 100,100,100,255 });
			DrawRectangleLinesEx({ mouse_position.x, mouse_position.y, (float)brush_size, (float)brush_size }, 1, Curent_mats.color);
		EndMode2D();

		DrawFPS(100, 10);
		DrawRectangle(10, 10, 60, 60, { 100,100,100,255 });
		DrawRectangle(15, 15, 50, 50, Curent_mats.color);
		DrawText(Curent_mats.name.c_str(), 18, 18, 3, BLACK);
		EndDrawing();
		
		
	}

	CloseWindow();
}




