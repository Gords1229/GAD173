#include "App.h"

int main()
{
	App game("Brick Level Editor", 1920, 1080, 32);

	if (!game.Init()) {
		printf("Game could not be started!");
		return 1;
	}
	else {
		game.Run();
	}
	return 0;
}