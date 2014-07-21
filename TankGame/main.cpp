#include "ExampleGame.h"
#include "MathUtils.h"


int main(int argc, char* argv[])
{
	ExampleGame* game = new ExampleGame();
	game->InitSDL();
	game->Run();
	game->Clean();
	return 0;
}