#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "SDL_image.h"

using namespace std;

bool sortObjects (Level::Object a,Level::Object b) 
{ 
	return (a.baseY < b.baseY);
}

Level::Level(string tileFilename, string objectFilename)
{
	loadObjectData(objectFilename);
	loadData(tileFilename);
}


Level::~Level(void)
{
	if(pTileSheet_)
	{
		SDL_FreeSurface(pTileSheet_);
	}

	for(int i = 0; i < levelTilesHigh_; ++i)
	{
		delete [] ppTileMapLayer_[i];
		delete [] ppCollisionLayer_[i];
	}

	delete [] ppTileMapLayer_;
	delete [] ppCollisionLayer_;

	for(int i = 0; i < objects_.size(); ++i)
	{
		//delete objects_[i];
	}

	ppTileMapLayer_ = NULL;
	ppCollisionLayer_ = NULL;
}

void Level::loadObjectData(std::string filename)
{
	ifstream objectInfo;
	objectInfo.open(filename, ios::in);

	if(!objectInfo.is_open())
	{
		objectInfo.close();
		cerr << "Error, can't open data file: " << filename << endl;
		return;
	}

	string buf; 
	while(objectInfo.good())
	{
		objectInfo >> buf;
		if(buf == "object")
		{
			objectInfo >> buf;
			stringstream ss(buf);

			Uint16 objectID;
			ObjectData newObject;
			ss >> objectID;

			objectInfo >> buf; //throw out token
			objectInfo >> buf; //get x;

			ss.clear();
			ss.str(buf);
			ss >> newObject.x;

			objectInfo >> buf; //throw out token
			objectInfo >> buf; //get y;

			ss.clear();
			ss.str(buf);
			ss >> newObject.y;


			objectInfo >> buf; //throw out token
			objectInfo >> buf; //get width;

			ss.clear();
			ss.str(buf);
			ss >> newObject.width;

			objectInfo >> buf; //throw out token
			objectInfo >> buf; //get height;

			ss.clear();
			ss.str(buf);
			ss >> newObject.height;

			objectData_.insert(std::pair<Uint16, ObjectData>(objectID, newObject));

		}
	}
}

void Level::loadData(std::string fileName)
{
	ifstream levelInfo;
	levelInfo.open(fileName, ios::in);

	if(!levelInfo.is_open())
	{
		levelInfo.close();
		cerr << "Error, can't open data file: " << fileName << endl;
		return;
	}

	string buf; 
	Uint16** arrayToInit = NULL;
	while(levelInfo.good())
	{
		levelInfo >> buf;
		if(buf == "width=")
		{
			levelInfo >> buf;
			stringstream ss(buf);
			ss >> levelTilesWide_;
		}
		else if(buf == "height=")
		{
			levelInfo >> buf;
			stringstream ss(buf);
			ss >> levelTilesHigh_;

			ppTileMapLayer_ = 
					new Uint16*[levelTilesHigh_];

			//ppTileObjectsLayer_ = 
			//		new Uint16*[levelTilesHigh_];

			ppCollisionLayer_ = 
					new Uint16*[levelTilesHigh_];
		}
		else if(buf == "tilewidth=")
		{
			levelInfo >> buf;
			stringstream ss(buf);
			ss >> tileWidth_;
		}
		else if(buf == "tileheight=")
		{
			levelInfo >> buf;
			stringstream ss(buf);
			ss >> tileHeight_;
		}
		else if(buf == "tileset=")
		{
			levelInfo >> buf; 
			pTileSheet_ = IMG_Load(buf.c_str());
		}
		else if(buf == "type=")
		{
			levelInfo >> buf;
			if(buf == "Tile")
			{
				arrayToInit = ppTileMapLayer_;
			}
			else if(buf == "ObjectLayer")
			{
				//load objects 
				//throw out data=
				levelInfo >> buf;
				for(int row = 0; row < levelTilesHigh_; ++row)
				{
					for(int col = 0; col < levelTilesWide_; ++col)
					{
						levelInfo >> buf;
						stringstream ss(buf);
						Uint16 id;
						ss >> id;
						if(id != 0)
						{
							//check for ObjectData before adding it
							if( objectData_.find(id) != objectData_.end())
							{
								Object newObject;
								newObject.id = id;
								newObject.pos.x = (float)(col * tileWidth_);
								newObject.pos.y = (float)(row * tileHeight_);
								newObject.baseY = newObject.pos.y + (float)objectData_[id].height;
								objects_.push_back(newObject);
							}
						}
					}
				}
			}
			else if(buf == "Collision")
			{
				arrayToInit = ppCollisionLayer_;
			}
		}
		else if(buf == "data=")
		{
			for(int row = 0; row < levelTilesHigh_; ++row)
			{
				arrayToInit[row] = new Uint16[levelTilesWide_];
				for(int col = 0; col < levelTilesWide_; ++col)
				{
					levelInfo >> buf;
					stringstream ss(buf);
					ss >> arrayToInit[row][col];
				}
			}
		}
	}
	srcTilesWide_ = pTileSheet_->w / tileWidth_;
	levelInfo.close();
	std::sort(objects_.begin(), objects_.end(), sortObjects);
}