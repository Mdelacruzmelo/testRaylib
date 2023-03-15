#include "raylib.h"
#include "utils.h"
#include "Game.h"
#include <string>
#include <sstream>

int main()
{
	InitAudioDevice();
	SetTargetFPS(30);

	Game game = Game();
	InitWindow(game.screenWidth, game.screenHeight, "raylib game template");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		switch (game.screen)
		{
		case TITLE: game.TitleScreen(); break;
		case LOADING: game.LoadingScreen(); break;
		case GAME: game.GameScreen(); break;
		case SUCCESS: game.SuccessScreen(); break;
		case FAILURE: game.FailureScreen(); break;
		default: break;
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}