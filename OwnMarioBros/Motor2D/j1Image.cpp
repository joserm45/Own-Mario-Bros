#include "j1Image.h"
#include "j1Render.h"

j1Image::j1Image() {}

j1Image::j1Image(iPoint pos, SDL_Rect rect)
{
	this->type = IMAGE;
	this->position = pos;
	this->atlas_pos = rect;
}
j1Image::~j1Image()
{

}

void j1Image::Draw()
{
	App->render->Blit(App->gui->atlas, camera_pos.x, camera_pos.y, &atlas_pos);
}
