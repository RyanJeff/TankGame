#include "Sprite.h"

void Sprite::Draw(SDL_Surface* surfaceToDrawOn)
{
	SDL_SetAlpha(pImage_, SDL_SRCALPHA, alpha_);
	SDL_Rect destR;
	destR.x = (Sint16)pos_.x - (srcRect_.w / 2);
	destR.y = (Sint16)pos_.y - (srcRect_.h / 2);
	SDL_BlitSurface(pImage_, &srcRect_, surfaceToDrawOn, &destR);
}

void Sprite::Draw(SDL_Surface* surfaceToDrawOn, const Vec2& cam)
{
	SDL_SetAlpha(pImage_, SDL_SRCALPHA, alpha_);
	SDL_Rect destR;
	destR.x = (Sint16)pos_.x - (srcRect_.w / 2) - cam.x;
	destR.y = (Sint16)pos_.y - (srcRect_.h / 2) - cam.y;
	SDL_BlitSurface(pImage_, &srcRect_, surfaceToDrawOn, &destR);
}