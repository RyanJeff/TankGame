#include "ExampleGame.h"

#include "Level.h"

void ExampleGame::loadAssets()
{
	pLevel_ = new Level("level1.dat", "TileData.dat");
	//------------Test code------------
//	SDL_Surface* temp = IMG_Load("main_character.png");
//	SDL_Surface* temp2 = SDL_DisplayFormatAlpha(temp);
	
//	SDL_FreeSurface(temp);

	/*SDL_Rect srcRect = SDL_Rect();
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 32;
	srcRect.h = 48;*/
	pTestSprite_ = new Player(Vec2(100.0f, 100.0f));
	camera_.x = pTestSprite_->GetPos().x - (SCREEN_WIDTH / 2.0f);
	camera_.y = pTestSprite_->GetPos().y - (SCREEN_HEIGHT / 2.0f);
	//-----------------------------------------------
}

Vec2 ExampleGame::getAARROverlap(const SDL_Rect& r1, const SDL_Rect& r2)
{
	Sint16 r1CentreX = r1.x + (r1.w / 2);
	Sint16 r1CentreY = r1.y + (r1.h / 2);
	Sint16 r2CentreX = r2.x + (r2.w / 2);
	Sint16 r2CentreY = r2.y + (r2.h / 2);

	Sint16 diffX = r1CentreX - r2CentreX;
	Sint16 diffY = r1CentreY - r2CentreY;
	Uint16 halfWidths = (r1.w + r2.w) / 2;
	Uint16 halfHeights = (r1.h + r2.h) / 2;

	Sint16 overlapX = halfWidths - abs(diffX);
	Sint16 overlapY = halfHeights - abs(diffY);

	if(overlapX > 0 &&
	   overlapY > 0)
	{
		//check to see which axis to correct on (smallest overlap)
		if(overlapX > overlapY)
		{
			//correct on y
			if(r1CentreY < r2CentreY)
			{
				return Vec2(0.0f, -overlapY);
			}
			else
			{
				return Vec2(0.0f, overlapY);
			}
		}
		else
		{
			//correct on x
			if(r1CentreX < r2CentreX)
			{
				return Vec2(-overlapX, 0.0f);
			}
			else
			{
				return Vec2(overlapX, 0.0f);
			}
		}
	}

	return Vec2(0.0f, 0.0f);
}

bool ExampleGame:: rectRectCollision(const SDL_Rect& r1, 
										const SDL_Rect& r2)
{
	Sint16 diffX = r1.x - r2.x;
	Sint16 diffY = r1.y - r2.y;
	Uint16 halfWidths = (r1.w + r2.w) / 2;
	Uint16 halfHeights = (r1.h + r2.h) / 2;

	if(abs(diffX) < halfWidths &&
	   abs(diffY) < halfHeights)
	{
		return true;
	}

	return false;
}

void ExampleGame::characterLevelCollision(Character* character)
{
	float x = character->GetPos().x - (character->GetWidth() / 2);
	float y = character->GetPos().y - (character->GetHeight() / 2);

	float w = character->GetWidth();
	float h = character->GetHeight();

	SDL_Rect charRect;
	charRect.x = (Sint16)x;
	charRect.y = (Sint16)y;
	charRect.w = (Sint16)w;
	charRect.h = (Sint16)h;

	Uint16 leftCol = charRect.x / pLevel_->GetTileWidth();
	Uint16 rightCol = (charRect.x + charRect.w) / pLevel_->GetTileWidth();
	Uint16 topRow = charRect.y / pLevel_->GetTileHeight();
	Uint16 bottomRow = (charRect.y + charRect.h) / pLevel_->GetTileHeight();

	for(int col = leftCol; col <= rightCol; ++col)
	{
		for(int row = topRow; row <= bottomRow; ++row)
		{
			if(pLevel_->GetCollisionData()[row][col] != 0)
			{
				SDL_Rect tileRect;
				tileRect.x = col * pLevel_->GetTileWidth();
				tileRect.y = row * pLevel_->GetTileHeight();
				tileRect.w = pLevel_->GetTileWidth();
				tileRect.h = pLevel_->GetTileHeight();

				Vec2 overlap = getAARROverlap(charRect, tileRect);
				//broad phase rect / rect check
				if(overlap.x != 0.0f || overlap.y != 0.0f)
				{
					//Temp / test -> tells us if the rect rect works
					pTestSprite_->SetPos(pTestSprite_->GetPos() +
										 overlap);
					pTestSprite_->SetVel(Vec2(0.0f, 0.0f));
					//-----------------------------------------------

					//do per pixel collision between the rectangles
					//if(perPixelCollision(charRect, character, tileRect))
						//handle the collision
				}
			}
		}
	}
}



float CAMERA_STIFFNESS = 500.0f;
void ExampleGame::updateCamera()
{
	//calculate centre of screen
	Vec2 centre;
	centre.x = camera_.x + (SCREEN_WIDTH / 2.0f);
	centre.y = camera_.y + (SCREEN_HEIGHT / 2.0f);
	
	//calculate vector to the player
	Vec2 toPlayer;
	toPlayer.x = pTestSprite_->GetPos().x - centre.x;
	toPlayer.y = pTestSprite_->GetPos().y - centre.y;

	float mag = VectorMath::Magnitude(&toPlayer);
	toPlayer = VectorMath::Normalize(&toPlayer);
	float d = mag / CAMERA_STIFFNESS;

	camera_ += toPlayer * d;

	camera_.x = max(0.0f, camera_.x);
	float levelRightEdge = pLevel_->GetLevelRight();
	camera_.x = min(levelRightEdge - SCREEN_WIDTH, camera_.x);

	camera_.y = max(0.0f, camera_.y);
	camera_.y = min(pLevel_->GetLevelBottom() - SCREEN_HEIGHT, camera_.y);
}

void ExampleGame::checkLevelBounds()
{
	Vec2 playerPos = pTestSprite_->GetPos();
	
	playerPos.x = max(pTestSprite_->GetWidth() / 2.0f, playerPos.x);
	playerPos.x = min(pLevel_->GetLevelRight() - pTestSprite_->GetWidth() / 2.0f,
					  playerPos.x);

	playerPos.y = max(pTestSprite_->GetHeight() / 2.0f, playerPos.y);
	playerPos.y = min(pLevel_->GetLevelBottom() - pTestSprite_->GetHeight() / 2.0f,
					  playerPos.y);

	pTestSprite_->SetPos(playerPos);
}


void ExampleGame::update(float deltaTime)
{
	pTestSprite_->Update(deltaTime);
	
	checkLevelBounds();

	characterLevelCollision(pTestSprite_);	

	updateCamera();
}

void ExampleGame::drawLevel()
{
	Uint16 lCol = camera_.x / pLevel_->GetTileWidth();
	Uint16 rCol = (camera_.x + SCREEN_WIDTH) / 
					pLevel_->GetTileWidth();

	Uint16 tRow = camera_.y / pLevel_->GetTileHeight();
	Uint16 bRow = (camera_.y + SCREEN_HEIGHT) / 
					pLevel_->GetTileHeight();

	Uint16 const * const * const mapData = pLevel_->GetMapData();
	//Uint16 const * const * const objectData = pLevel_->GetObjectData();
	//Uint16 const * const * const collisionData = pLevel_->GetCollisionData();
	
	for(Uint16 col = lCol; col <= rCol; ++col)
	{
		for(Uint16 row = tRow; row <= bRow; ++row)
		{
			renderLevelTile(row, col, mapData);
			//renderLevelTile(row, col, objectData);
			//renderLevelTile(row, col, collisionData);
		}
	}

	bool playerDrawn = false;
	float playerBase = pTestSprite_->GetPos().y + (pTestSprite_->GetHeight() / 2.0f);
	const std::vector<Level::Object>& objects = pLevel_->GetObjects();
	for(int i = 0; i < objects.size(); ++i)
	{
		if(!playerDrawn && objects[i].baseY > playerBase)
		{
			pTestSprite_->Draw(pDisplay_, camera_);
			playerDrawn = true;
		}
		const Level::ObjectData& objectData = pLevel_->GetObjectData().at(objects[i].id);
		SDL_Rect srcRect;
		srcRect.x = objectData.x;
		srcRect.y = objectData.y;
		srcRect.w = objectData.width;
		srcRect.h = objectData.height;	

		SDL_Rect dstRect;
		dstRect.x = objects[i].pos.x - camera_.x;
		dstRect.y = objects[i].pos.y - camera_.y;
		dstRect.w = srcRect.w;
		dstRect.h = srcRect.h;

		SDL_BlitSurface(pLevel_->GetTileSheet(), &srcRect, 
						pDisplay_, &dstRect);
	}

	if(!playerDrawn)
	{
		pTestSprite_->Draw(pDisplay_, camera_);
		playerDrawn = true;
	}
}

void ExampleGame::renderLevelTile(Uint16 row, Uint16 col, 
								  Uint16 const * const * const levelData)
{
	if(levelData[row][col] != 0)
	{
		Uint16 index = levelData[row][col] - 1;					
				
		SDL_Rect srcRect;
		Uint16 tileSheetCol = index % pLevel_->GetSrcTilesWide();
		srcRect.x = tileSheetCol * pLevel_->GetTileWidth();

		Uint16 tileSheetRow = index / pLevel_->GetSrcTilesWide();
		srcRect.y = tileSheetRow * pLevel_->GetTileHeight();

		srcRect.w = pLevel_->GetTileWidth();
		srcRect.h = pLevel_->GetTileHeight();

		SDL_Rect dstRect;
		dstRect.x = (col * pLevel_->GetTileWidth()) - camera_.x;
		dstRect.y = (row * pLevel_->GetTileHeight()) - camera_.y;
		dstRect.w = pLevel_->GetTileWidth();
		dstRect.h = pLevel_->GetTileHeight();

		SDL_BlitSurface(pLevel_->GetTileSheet(), &srcRect, 
						pDisplay_, &dstRect);
	}
}

void ExampleGame::draw()
{
	drawLevel();
	Game::draw();
}

void ExampleGame::onKeyDown(Uint16 key)
{
	pTestSprite_->OnKeyDown(key);
}

void ExampleGame::onKeyUp(Uint16 key)
{
	pTestSprite_->OnKeyUp(key);
}