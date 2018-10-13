#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	if (current_lvl == 1)
	{
		App->map->Load("lvl1.tmx");
		App->audio->PlayMusic("audio/music/lvl_1.ogg");
		App->audio->LoadFx("audio/music/jump.wav");
		App->audio->LoadFx("audio/music/double_jump.wav"); 
		death_sound = App->audio->LoadFx("audio/music/life_lost.ogg");
		level_sound = App->audio->LoadFx("audio/music/level_clear.ogg");
	}
	else if (current_lvl == 2)
	{
		App->map->Load("lvl2.tmx");
		App->audio->PlayMusic("audio/music/lvl_2.ogg");
		App->audio->LoadFx("audio/jump.wav");
		App->audio->LoadFx("audio/double_jump.wav");
		death_sound = App->audio->LoadFx("audio/music/life_lost.ogg");
		level_sound = App->audio->LoadFx("audio/music/level_clear.ogg");
	}
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadLevel(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		LoadLevel(current_lvl);
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		LoadLevel(2);
	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god_mode = !god_mode;

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (App->map->logic == false)
		{
			App->map->logic = true;
		}
		else if (App->map->logic == true)
		{
			App->map->logic = false;
		}
	}

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());*/
	//p2SString title("%c", *App->win->SetTitle(title.GetString()));
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadLevel(int current_level)
{

	App->map->UnloadMap();
	App->player->CleanUp();
	App->player->Start();
	if (current_level == 1)
	{
		this->current_lvl = current_level;
		App->map->Load("lvl1.tmx");
		App->audio->PlayMusic("audio/music/lvl_1.ogg");
	}
	else if (current_level == 2)
	{
		this->current_lvl = current_level;
		App->map->Load("lvl2.tmx");
		App->audio->PlayMusic("audio/music/lvl_2.ogg");
	}


	App->render->camera.x = 0;
	App->render->camera.y = 0;
	/*pugi::xml_node node;
	
	pugi::xml_parse_result result = config.load_file("save_game.xml");
	if (result != NULL)
	{
		node = config.child("game_state");


		App->player->CleanUp();
		App->player->Awake(config);

	}
	else
	{

	}*/


}