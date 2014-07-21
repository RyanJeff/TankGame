#include "Player.h"

#include <math.h>

const float FRICTION = 1.0f; // the amount of velocity lost per second (as a ratio)

void Player::Update(float deltaTime)
{
	velocity_ *= 0.97f;
	velocity_ += inputDir_ * (ACCEL * deltaTime);
	pos_ += velocity_ * deltaTime;
	SpriteAnimation::Update(deltaTime);
}

void Player::OnKeyDown(Uint16 key)
{
	switch(key)
	{
	case SDLK_UP:
		inputDir_.y = -1.0f;
		break;

	case SDLK_DOWN:
		inputDir_.y = 1.0f;
		break;

	case SDLK_RIGHT:
		inputDir_.x = 1.0f;
		break;

	case SDLK_LEFT:
		inputDir_.x = -1.0f;
		break;
	}

	inputDir_ = VectorMath::Normalize(&inputDir_);

	if(!isPlaying_)
	{
		Play(true);
	}

	updateCurrAnim();
}

void Player::OnKeyUp(Uint16 key)
{
	switch(key)
	{
	case SDLK_UP:
		if(inputDir_.y < -EPSILON)
		{
			inputDir_.y = 0.0f;
		}
		break;

	case SDLK_DOWN:
		if(inputDir_.y > EPSILON)
		{
			inputDir_.y = 0.0f;
		}
		break;

	case SDLK_RIGHT:
		if(inputDir_.x > EPSILON)
		{
			inputDir_.x = 0.0f;
		}
		break;

	case SDLK_LEFT:
		if(inputDir_.x < -EPSILON)
		{
			inputDir_.x = 0.0f;
		}
		break;
	}

	updateCurrAnim();
}

void Player::updateCurrAnim()
{
	if(inputDir_.x > EPSILON)
	{
		SetCurrAnim(MOVE_RIGHT_ANIM);
	}
	else if(inputDir_.x < -EPSILON)
	{
		SetCurrAnim(MOVE_LEFT_ANIM);
	}
	else if(inputDir_.y > EPSILON)
	{
		SetCurrAnim(MOVE_DOWN_ANIM);
	}
	else if(inputDir_.y < -EPSILON)
	{
		SetCurrAnim(MOVE_UP_ANIM);
	}
	else
	{
		Stop();
		currFrame_ = startFrame_;
	}
}