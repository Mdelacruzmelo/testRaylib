#pragma once
#include "raylib.h"
#include "utils.h"

class Game {

public:
	Game();

	// Screen
	const int screenWidth = 1080;
	const int screenHeight = 720;
	E_Screen screen = TITLE;
	Vector2 cursorPosition = { -100.0f, -100.0f };
	Rectangle Cursor1;
	Rectangle Cursor2;

	// Loading
	int LOADING_TIME = 10;
	float timeInLoading = 0;
	int loadingBarPercentage = 0;

	// Gameplay
	int GAME_TIME = 30;
	float timeGaming = 0;
	float gamingBarPercentage = 0;
	int savedSeconds = 0;
	int pendingSeconds = 0;
	int totalTextures = 36;
	int timeLeft;
	bool gameSuccess;
	bool gameSaved = false;
	Texture2D* textures = new Texture2D[totalTextures];

	// Effects
	Sound hoverSound;
	Sound clickSound;
	int numberHovered;
	int numberClicked;
	void SoundHover(int numberHovered);
	void SoundClick(int numberClicked);

	// Screens

	void TitleScreen();
	void LoadingScreen();
	void GameScreen();
	void SuccessScreen();
	void FailureScreen();

	void InitTextures();
	Vector2* GetRandomVectors();
	int* GetRandomNumbersExcept(int numbersToAvoid1, int numbersToAvoid2, int numbersToAvoid3);

	void DrawCursor();
	bool IsHoveringImage(Vector2 pos);
	bool ClickedOnImage(Vector2 pos);

	SaveData LoadData();
	void SaveRecord(int record, char* savedData);
	int* GetRecords(char* savedData);

};
