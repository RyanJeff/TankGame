#include "SpriteAnimation.h"

Uint16 numFrames_;
	Uint16 currFrame_;
	Uint16 startFrame_;

	Uint16 framesWide_;
	Uint16 framesHigh_;
	Uint16 frameWidth_;
	Uint16 frameHeight_;

	float frameTime_;
	float frameRate_;
	
	bool isLooped_;
	bool isPlaying_;

SpriteAnimation::SpriteAnimation(void) : Sprite(),
	frameWidth_(0),
	frameHeight_(0),
	numFrames_(0),
	startFrame_(0),
	currFrame_(0),
	frameRate_(0.0f),
	frameTime_(0.0f),
	framesWide_(0),
	framesHigh_(0),
	isLooped_(false),
	isPlaying_(false)
{
}

SpriteAnimation::SpriteAnimation(SDL_Surface* image, const Vec2& pos, SDL_Rect srcRect,
								Uint8 alpha, Uint16 frameWidth, Uint16 frameHeight, 
								Uint16 startFrame, Uint16 numFrames, 
								float frameRate) 
:	Sprite(image, pos, srcRect, alpha),
	frameWidth_(frameWidth),
	frameHeight_(frameHeight),
	numFrames_(numFrames),
	startFrame_(startFrame),
	currFrame_(0),
	frameRate_(frameRate),
	frameTime_(0.0f),
	framesWide_(0),
	framesHigh_(0),
	isLooped_(false),
	isPlaying_(false)
{
	initAnim();
}


SpriteAnimation::~SpriteAnimation(void)
{
}

void SpriteAnimation::Update(float deltaTime)
{
	if(isPlaying_)
	{
		frameTime_ += deltaTime;
		if(frameTime_ > frameRate_)
		{
			frameTime_ -= frameRate_;
			currFrame_++;

			if(currFrame_ >= numFrames_)
			{
				currFrame_ = 0;
				if(!isLooped_)
				{
					isPlaying_ = false;
				}

			}
			updateSrcFrame();
		}
	}

}

void SpriteAnimation::updateSrcFrame()
{
	Uint16 absIndex = startFrame_ + currFrame_;

	Uint16 col = absIndex % framesWide_;
	srcRect_.x = col * frameWidth_;

	Uint16 row = absIndex / framesWide_;
	srcRect_.y = row * frameHeight_;

	srcRect_.w = frameWidth_;
	srcRect_.h = frameHeight_;
}

void SpriteAnimation::initAnim()
{
	framesWide_ = pImage_->clip_rect.w / frameWidth_;
	framesHigh_ = pImage_->clip_rect.h / frameHeight_;
	updateSrcFrame();
}

