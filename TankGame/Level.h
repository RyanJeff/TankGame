#pragma once

#include <string>
#include <map>
#include <vector>
#include "SDL.h"

#include "MathUtils.h"

class Sprite;

class Level
{
public:
struct ObjectData
{
	Uint16 x;
	Uint16 y;
	Uint16 width;
	Uint16 height;
};

struct Object
{
	Uint16 id;
	Vec2 pos;
	float baseY;
};

protected:
	SDL_Surface* pTileSheet_;
	Uint16 tileWidth_;
	Uint16 tileHeight_;
	Uint16 levelTilesWide_;
	Uint16 levelTilesHigh_;
	Uint16 srcTilesWide_;

	
	Uint16** ppTileMapLayer_;
	Uint16** ppCollisionLayer_;

	std::map<Uint16, ObjectData> objectData_;
	std::vector<Object> objects_;
	



	void loadData(std::string fileName);
	void loadObjectData(std::string filename);

public:
	Level::Level(std::string fileName,
				 std::string objectFilename);
	~Level(void);

	Uint16 GetTileWidth()
	{
		return tileWidth_;
	}

	Uint16 GetTileHeight()
	{
		return tileHeight_;
	}

	Uint16 const * const * const GetMapData() const
	{
		return ppTileMapLayer_;
	}

	Uint16 const * const * const GetCollisionData() const
	{
		return ppCollisionLayer_;
	}

	Uint16 GetLevelTilesWide()
	{
		return levelTilesWide_;
	}

	SDL_Surface* GetTileSheet()
	{
		return pTileSheet_;
	}

	const std::map<Uint16, ObjectData>& GetObjectData() const
	{
		return objectData_;
	}

	const std::vector<Object>& GetObjects() const
	{
		return objects_;
	}

	float GetLevelRight() const
	{
		return (float)(tileWidth_ * levelTilesWide_);
	}

	float GetLevelBottom() const
	{
		return (float)(tileHeight_ * levelTilesHigh_);
	}

	Uint16 GetSrcTilesWide() const
	{
		return srcTilesWide_;
	}
};

