#include "raylib.h"
#include "utils.h"
#include "Game.h"
#include <string>
#include <sstream>

int main(void)
{
	InitAudioDevice();
	SetTargetFPS(30);

	Game G = Game();
	InitWindow(G.screenWidth, G.screenHeight, "raylib game template");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		switch (G.screen)
		{
		case TITLE: G.TitleScreen(); break;
		case LOADING: G.LoadingScreen(); break;
		case GAME: G.GameScreen(); break;
		case SUCCESS: G.SuccessScreen(); break;
		case FAILURE: G.FailureScreen(); break;
		default: break;
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}