#include "j1Player.h"
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
#include "j1Collision.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

	Load_Animation();
	
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& node)
{
	bool ret = true;
	if (node != NULL)
	{
		position.x = node.child("player").child("position").attribute("x").as_int();
		position.y = node.child("player").child("position").attribute("y").as_int();;
	}
	status = IDLE;
	//current = &right_idle;
	
	return ret;
}

bool j1Player::Start()
{
	position.x = 112;
	position.y = 176;
	
	//load texture
	text_player = App->tex->Load("textures/mario.png");

	//load collider
	collider_player = App->collision->AddCollider({ (int)position.x,(int)position.y,MARIO_WIDTH,MARIO_HEIGHT }, COLLIDER_PLAYER, this);
	
	player_quadrant_1.x = position.x / TILE_WIDTH;
	player_quadrant_2.x = (position.x + MARIO_WIDTH) / TILE_WIDTH;

	player_quadrant_1.y = position.y / TILE_WIDTH;
	player_quadrant_2.y = (position.y + MARIO_HEIGHT) / TILE_WIDTH;
	return true;
}

bool j1Player::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Player::Update(float dt)
{
	bool ret = true;

	//Movement
	moving = false;
	
	//duck
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		status = DUCK;
		moving = true;
	}

	//right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if (status != DUCK)
		{
			status = RIGHT;
			if (App->map->Walkability() == true)
			{
				position.x += PLAYER_SPEED;
			}

			moving = true;
			back = false;
		}
	}
	//left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (status != DUCK)
		{
			status = LEFT;
			if (App->map->Walkability() == true)
			{
				position.x -= PLAYER_SPEED;
			}

			moving = true;
			back = true;
		}
	}	

	//die
	//to check if animation works for the moment
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		status = DIE;
		position.y -= 150;
		moving = false;
		App->audio->StopMusic();
		App->audio->PlayFx(App->scene->death_sound, 0);
	}

	//respawn
	//to check if it works for the moment
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		status = IDLE;
		if (App->scene->current_lvl == 1)
		{
			position.x = 86;
			position.y = 174;
			App->audio->PlayMusic("audio/music/lvl_1.ogg");
		}
		else if (App->scene->current_lvl == 2)
		{
			App->audio->PlayMusic("audio/music/lvl_2.ogg");
		}
	}


	//god mode
	if (App->scene->god_mode == true)
	{
	
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			jumping = true;
			jump_height = position.y - 40;
			jump1_on = true;
		}
		

		/*if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			if (App->map->Walkability() == true)
			{
				position.y += PLAYER_SPEED * dt;
			}
		}*/

	}

	//status check
	if (moving == false && status != DIE)
	{
		status = IDLE;
	}

	if (!jump1_on)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			App->audio->PlayFx(1);
			jumping = true;
			jump_height = position.y - 40;
			jump1_on = true;
		}
	}
	else if (!jump2_on)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			App->audio->PlayFx(2);
			jumping = true;
			jump_height = position.y - 25;
			jump2_on = true;
		}
	}

	CameraMovement();

	/*if (jumping_over == false)
	{
		if (jumping == true)
		{
			if (count_jump < 13)
			{
				position.y -= PLAYER_JUMP;
				count_jump++;
				status = JUMP;
			}
			else
			{
				jumping_over = true;
			}
		}
		
	}
	else
	{
		if (Falling() == true)
		{
			position.y += GRAVITY;
			status = JUMP;
		}
	}*/
	if (dead != true)
	{
		
		if (jumping != false)
		{
 			if (Jump() != false)
			{
				position.y -= 150 * dt;
			}
		}
		else
		{
			if (Falling())
			{
				position.y += 150 * dt;
			}
		}
				
	}
	if (dead == true)
	{
		dead = false;
		moving = false;
		App->audio->StopMusic();
		App->scene->LoadLevel(App->scene->current_lvl);
		
	}
	
	

	collider_player->SetPos(position.x, position.y);

	
	return ret;
}

bool j1Player::Jump()
{
	bool ret;
	status = JUMP;
	if (position.y > jump_height)
	{
		ret = true;
	}
	else
	{
		jumping = false;
		ret = false;
		jump_height = 0;
		
	}
	return ret;
}
bool j1Player::PostUpdate()
{
	bool ret = true;

	Draw();
	//Blit player
	App->render->Blit(text_player, position.x, position.y, &current->GetCurrentFrame());
	
	player_quadrant_1.x = position.x / TILE_WIDTH;
	player_quadrant_2.x = (position.x + MARIO_WIDTH) / TILE_WIDTH;

	player_quadrant_1.y = position.y / TILE_WIDTH;
	player_quadrant_2.y = (position.y + MARIO_HEIGHT) / TILE_WIDTH;
	
	return ret;
}

bool j1Player::CleanUp() 
{
	bool ret = true;

	collider_player->to_delete = true;
	SDL_DestroyTexture(text_player);
	return ret;
}

bool j1Player::Save(pugi::xml_node&) const
{
	bool ret = true;
	//save 
	return ret;
}

bool j1Player::Load(pugi::xml_node&)
{
	bool ret = true;
	//load
	return ret;
}

void j1Player::CameraMovement()
{
	int relative_pos_player = position.x + App->render->camera.x;

	if (relative_pos_player < App->render->border)
	{
		App->render->camera.x = -(position.x - App->render->border);
	}
	if (relative_pos_player > App->render->camera.w - App->render->border)
	{
		App->render->camera.x = -(position.x - (App->render->camera.w - App->render->border));
	}
	if (App->render->camera.x > 0)
	{
		App->render->camera.x = 0;
	}
	if (App->render->camera.x < -2555)
	{
		App->render->camera.x = -2555;
	}
}

void j1Player::Draw()
{
	//Draw Animations
	switch (status)
	{
		case IDLE:
		{
			if (App->scene->god_mode == false) 
			{
				if (back == false)
				{
					current = &right_idle;
					break;
				}
				else if (back == true)
				{
					current = &left_idle;
					break;
				}
			}
			else if (App->scene->god_mode == true)
			{
				if (back == false)
				{
					current = &god_right_idle;
					break;
				}
				else if (back == true)
				{
					current = &god_left_idle;
					break;
				}
			}
		}
		case LEFT:
		{
			if (App->scene->god_mode == false)
			{
				current = &move_left;
				break;
			}
			else if (App->scene->god_mode == true)
			{
				current = &god_move_left;
				break;
			}
		}
		case RIGHT:
		{
			if (App->scene->god_mode == false)
			{
				current = &move_right;
				break;
			}
			else if (App->scene->god_mode == true)
			{
				current = &god_move_right;
				break;
			}
		}
		case JUMP:
		{
			if (App->scene->god_mode == false)
			{
				if (back == false)
				{
					current = &right_jump;
					break;
				}
				else if (back == true)
				{
					current = &left_jump;
					break;
				}
			}
			else if (App->scene->god_mode == true)
			{
				if (back == false)
				{
					current = &god_right_jump;
					break;
				}
				else if (back == true)
				{
					current = &god_left_jump;
					break;
				}
			}
		}
		case DUCK:
		{
			if (App->scene->god_mode == false)
			{
				if (back == false)
				{
					current = &right_duck;
					break;
				}
				else if (back == true)
				{
					current = &left_duck;
					break;
				}
			}
			else if (App->scene->god_mode == true)
			{
				if (back == false)
				{
					current = &god_right_duck;
					break;
				}
				else if (back == true)
				{
					current = &god_left_duck;
					break;
				}
			}
		}
		case DIE:
		{
			current = &die;
		}
	}

}

void j1Player::Load_Animation()
{
	//MARIO

	//idle
	right_idle.PushBack({ 258,1,16,32 });
	left_idle.PushBack({ 239,1,16,32 });

	//Walk right
	move_right.PushBack({ 296,3,16,30 });
	move_right.PushBack({ 315,2,14,31 });
	move_right.PushBack({ 331,1,16,32 });
	move_right.speed = ANIMATION_SPEED;

	//Walk left
	move_left.PushBack({ 201,3,16,30 });
	move_left.PushBack({ 184,2,14,31 });
	move_left.PushBack({ 166,1,16,32 });
	move_left.speed = ANIMATION_SPEED;

	//Jump 
	//right_jump.PushBack({ 350,1,16,32 });
	right_jump.PushBack({ 369,1,16,32 });
	//right_jump.PushBack({ 388,1,16,32 });
	//right_jump.speed = ANIMATION_SPEED;
	//right_jump.loop = 0;

	//left_jump.PushBack({ 147,1,16,32 });
	left_jump.PushBack({ 128,1,16,32 });
	//left_jump.PushBack({ 111,1,16,32 });
	//left_jump.speed = ANIMATION_SPEED;
	//left_jump.loop = 0;

	//duck
	right_duck.PushBack({ 277, 1, 16, 32 });
	left_duck.PushBack({ 220, 1, 16, 32 });

	//die 
	die.PushBack({ 516, 1, 24, 34 });
	die.PushBack({ 540, 1, 24, 34 });
	die.PushBack({ 564, 1, 24, 34 });
	die.PushBack({ 588, 1, 24, 34 });
	die.PushBack({ 612, 1, 24, 34 });
	die.PushBack({ 636, 1, 24, 34 });
	die.PushBack({ 660, 1, 24, 34 });
	die.speed = ANIMATION_SPEED;


	//GOD MODE MARIO

	//idle
	god_right_idle.PushBack({ 258,129,16,32 });
	god_left_idle.PushBack({ 239,129,16,32 });

	//Walk right
	god_move_right.PushBack({ 296,131,16,30 });
	god_move_right.PushBack({ 315,130,14,31 });
	god_move_right.PushBack({ 331,129,16,32 });
	god_move_right.speed = ANIMATION_SPEED;

	//Walk left
	god_move_left.PushBack({ 201,131,16,30 });
	god_move_left.PushBack({ 184,130,14,31 });
	god_move_left.PushBack({ 166,129,16,32 });
	god_move_left.speed = ANIMATION_SPEED;

	//Jump 
	//god_right_jump.PushBack({ 350,129,16,32 });
	god_right_jump.PushBack({ 369,129,16,32 });
	//god_right_jump.PushBack({ 388,129,16,32 });
	//god_right_jump.speed = ANIMATION_SPEED;
	//god_right_jump.loop = 0;

	//god_left_jump.PushBack({ 147,129,16,32 });
	god_left_jump.PushBack({ 128,129,16,32 });
	//god_left_jump.PushBack({ 111,129,16,32 });
	//god_left_jump.speed = ANIMATION_SPEED;
	//god_left_jump.loop = 0;

	//duck
	god_right_duck.PushBack({ 277, 129, 16, 32 });
	god_left_duck.PushBack({ 220, 129, 16, 32 });
	



	/*
	//LITTLE MARIO
	right_idle = { 277,44,12,16 };

	move_right.PushBack({ 54,181,27,32 });
	move_right.PushBack({ 93,181,27,32 });
	move_right.PushBack({ 11,220,27,32 });
	move_right.PushBack({ 54,218,27,32 });
	move_right.PushBack({ 91,219,27,32 });
	
	jump = { 126,220,34,32 };
	
	//die = { 127,263,32,28 };
	*/

}

bool j1Player::Falling()
{
	bool ret = false;
	p2List_item<Layer*>* iterator;
	p2List_item<Layer*>* layer = nullptr;

	for (iterator = App->map->data.layers.start; iterator != NULL; iterator = iterator->next)
	{
		if (iterator->data->name == "logic")
		{
			layer = iterator;
		}
	}

	//uint nextGid = fakeLayer->data->GetGid(player_x,player_y);
    uint* nextGid1 = &layer->data->gid[ player_quadrant_1.x + player_quadrant_2.y * layer->data->width];
	uint* nextGid2 = &layer->data->gid[ player_quadrant_2.x + player_quadrant_2.y * layer->data->width];


	if (*nextGid1 == 0 && *nextGid2 == 0)
	{
		ret = true;
		jump1_on = true;
	}
	if (*nextGid1 == 650 || *nextGid2 == 650)
	{
		
		//jumping = false;
		jump2_on = false;
		jump1_on = false;
		jumping_over = false;
	}

	if (*nextGid1 == 679 || *nextGid2 == 679)
	{
		if (App->scene->god_mode != true)
		dead = true;
	}

	return ret;
}