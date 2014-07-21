#pragma once
#include "character.h"
class Player :
	public Character
{
protected:
	const char* MOVE_LEFT_ANIM;
	const char* MOVE_RIGHT_ANIM;
	const char* MOVE_DOWN_ANIM;
	const char* MOVE_UP_ANIM;

	const float ACCEL; //player acceleration in pixels per second per second
	const float EPSILON;

	Vec2 inputDir_;

	void updateCurrAnim();
public:
	//Player(Vec2 pos) : Character(PLAYER, pos)
	Player(const Vec2& pos) : Character(pos),
		MOVE_LEFT_ANIM("RunLeft"),
		MOVE_RIGHT_ANIM("RunRight"),
		MOVE_DOWN_ANIM("RunDown"),
		MOVE_UP_ANIM("RunUp"),
		ACCEL(3000.0f),
		EPSILON(0.0001f)
	{
		inputDir_ = Vec2();
		LoadCharacter();
	}
	~Player(void){}

	//Overrides LoadCharacter so that it always loads player data by default
	virtual void LoadCharacter(CharacterType type)
	{
		Character::LoadCharacter(Character::PLAYER);
	}

	void LoadCharacter()
	{
		Character::LoadCharacter(Character::PLAYER);
	}

	virtual void OnKeyDown(Uint16 key);
	virtual void OnKeyUp(Uint16 key);

	virtual void Update(float deltaTime);
};

