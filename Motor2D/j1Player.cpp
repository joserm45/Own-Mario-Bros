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

j1Player::j1Player() : j1Module()
{
	name.create("player");

	//little mario
	//sprite_idle = { 277,44,12,16 };

	right_idle.PushBack({ 258,1,16,32 });
	left_idle.PushBack({ 239,1,16,32 });

	//Walk right
	move_right.PushBack({ 296,3,16,30 });
	move_right.PushBack({ 315,2,14,31 });
	move_right.PushBack({ 331,1,16,32 });
	move_right.speed = ANIMATION_SPEED;
	/*Litlle Mario
	move_right.PushBack({54,181,27,32});
	move_right.PushBack({ 93,181,27,32 });
	move_right.PushBack({ 11,220,27,32 });
	move_right.PushBack({ 54,218,27,32 });
	move_right.PushBack({ 91,219,27,32 });
	*/
	
	//Walk left
	move_left.PushBack({ 201,3,16,30 });
	move_left.PushBack({ 184,2,14,31 });
	move_left.PushBack({ 166,1,16,32 });
	move_left.speed = ANIMATION_SPEED;
	//Jump 
	right_jump.PushBack({ 369,2,16,32 });
	left_jump.PushBack({ 128,2,16,32 });

	//little mario
	//jump = { 126,220,34,32 };

	//Die 
	die.PushBack({ 277, 11, 16, 22 });

	//Litlle mario
	//die = { 127,263,32,28 };
	
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node&)
{
	bool ret = true;
	position.x = 86;
	position.y = 174;
	//current = &sprite_idle;
	status = IDLE;
	return ret;
}

bool j1Player::Start()
{
	
	//load texture
	text_player = App->tex->Load("textures/characters.png");

	//load collider

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
	//position.y += gravity;

	//right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		position.x += PLAYER_SPEED;
		status = RIGHT;
		moving = true;
		back = false;
	}
	//left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		position.x -= PLAYER_SPEED;
		status = LEFT;
		moving = true;
		back = true;
	}
	//jump
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		position.y -= PLAYER_JUMP;
		status = JUMP;
		moving = true;
	}
	//down
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		position.y += PLAYER_SPEED;
		status = DIE;
		moving = true;
	}

	//status check
	if (moving == false)
	{
		status = IDLE;
	}

	CameraMovement();

	return ret;
}

bool j1Player::PostUpdate()
{
	bool ret = true;

	Draw();
	//Blit
	App->render->Blit(text_player, position.x, position.y, &current->GetCurrentFrame());

	return ret;
}

bool j1Player::CleanUp() 
{
	bool ret = true;

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
	if (App->render->camera.x < -2800)
	{
		App->render->camera.x = -2800;
	}
}

void j1Player::Draw()
{
	//Load Animations
	switch (status)
	{
		case IDLE:
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
		case LEFT:
		{
			current = &move_left;
			break;
		}
		case RIGHT:
		{
			current = &move_right;
			break;
		}
		case JUMP:
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
		case DIE:
		{
			current = &die;
			break;
		}
	}

}