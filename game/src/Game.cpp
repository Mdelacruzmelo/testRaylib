#pragma once
#include "Game.h"
#include "raylib.h"
#include <string>
#include <sstream>

Game::Game() {
	hoverSound = LoadSound("resources/sounds/Hover_short.wav");
	clickSound = LoadSound("resources/sounds/Click.wav");
}

void Game::TitleScreen() {

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
	DrawText("Encuentra al objeto escondido", 150, 180, 36, WHITE);
	DrawText("Pulsa ENTER para empezar", 150, 220, 20, WHITE);

	if (IsKeyPressed(KEY_ENTER))
	{
		screen = LOADING;
	}

}

void Game::LoadingScreen() {

	timeInLoading += GetFrameTime();
	float GrowAlpha = (float)(timeInLoading / LOADING_TIME);
	float BarWidth = GetScreenWidth() * GrowAlpha;

	DrawRectangle(0, 0, (int)BarWidth, 30, WHITE);
	DrawText(
		TextFormat("loading: %d s", (LOADING_TIME - (int)(GrowAlpha * LOADING_TIME))),
		20,
		40,
		15,
		WHITE);

	DrawText("Tienes que encontrar 3 objetos", 150, 110, 24, WHITE);
	DrawRectangle(180, 150, 80, 80, WHITE);
	DrawRectangle(265, 150, 80, 80, WHITE);
	DrawRectangle(350, 150, 80, 80, WHITE);

	DrawText("Si los encuenras antes de que acabe el tiempo", 150, 300, 24, WHITE);
	DrawText("acumular�s tiempo para el siguiente nivel", 150, 335, 24, WHITE);
	DrawText("+10s", 180, 370, 40, GREEN);

	DrawText("Marca una puntuacion insuperable al terminar los 3 niveles", 150, 490, 24, WHITE);
	DrawText("12000", 180, 520, 40, YELLOW);

	loadingBarPercentage = (int)((BarWidth * 100) / GetScreenWidth());
	if (IsKeyPressed(KEY_ENTER) || loadingBarPercentage >= 100)
	{
		screen = GAME;
	}
}

void Game::GameScreen() {

	// -------------- Load textures -------------- 

	static bool texturesLoaded = false;
	if (!texturesLoaded) {
		InitTextures();
		texturesLoaded = true;
	}

	// -------------- Draw loading bar -------------- 

	timeGaming += GetFrameTime();
	float GrowGaminAlpha = timeGaming / GAME_TIME;
	float GameBarWidth = GetScreenWidth() * GrowGaminAlpha;

	// Si no casteo a int, sale un n�mero super raro :/
	int timeLeft = GAME_TIME - (int)(GrowGaminAlpha * GAME_TIME);

	DrawRectangle(0, 0, (int)GameBarWidth, 30, WHITE);
	DrawText(
		TextFormat("%d s", timeLeft),
		970,
		40,
		24,
		WHITE);

	// -------------- Draw Targets --------------

	static int textureNumber1 = GetRandomValue(0, (totalTextures - 1));
	static int textureNumber2 = GetRandomValue(0, (totalTextures - 1));
	static int textureNumber3 = GetRandomValue(0, (totalTextures - 1));
	static bool fruit1Found = false;
	static bool fruit2Found = false;
	static bool fruit3Found = false;
	gameSuccess = fruit1Found && fruit2Found && fruit3Found;

	while (textureNumber1 == textureNumber2) {
		textureNumber2 = GetRandomValue(0, (totalTextures - 1));
	}
	while (textureNumber3 == textureNumber1 || textureNumber3 == textureNumber2) {
		textureNumber3 = GetRandomValue(0, (totalTextures - 1));
	}

	DrawRectangleLines(40, 50, 80, 80, ORANGE);
	if (!fruit1Found) DrawTexture(textures[textureNumber1], 50, 50, WHITE);

	DrawRectangleLines(125, 50, 80, 80, ORANGE);
	if (!fruit2Found) DrawTexture(textures[textureNumber2], 135, 50, WHITE);

	DrawRectangleLines(210, 50, 80, 80, ORANGE);
	if (!fruit3Found) DrawTexture(textures[textureNumber3], 220, 50, WHITE);

	// -------------- Draw Fruits --------------

	static float startX = 100.f;
	static float startY = 200.f;
	static int count = 0;
	static Vector2* randomPositions = GetRandomVectors();
	static int* randomNumbers = GetRandomNumbersExcept(textureNumber1, textureNumber2, textureNumber3);

	for (int i = 0; i < 8; i++) {

		for (int j = 0; j < 5; j++) {

			if (count == 36) count = 0;

			Vector2 pos = Vector2{ (startX + (85 * i)), (startY + (85 * j)) };

			if (IsHoveringImage(randomPositions[count])) {

				DrawRectangleLines((int)randomPositions[count].x, (int)randomPositions[count].y, 60, 60, ORANGE);
				SoundHover(count);

				if (ClickedOnImage(randomPositions[count])) {

					DrawRectangleLines((int)randomPositions[count].x - 5, (int)randomPositions[count].y - 5, 70, 70, ORANGE);
					DrawRectangleLines((int)randomPositions[count].x - 10, (int)randomPositions[count].y - 10, 80, 80, ORANGE);

					if (
						count == textureNumber1 ||
						count == textureNumber2 ||
						count == textureNumber3
						) {

						SoundClick(count);

						if (count == textureNumber1) fruit1Found = true;
						if (count == textureNumber2) fruit2Found = true;
						if (count == textureNumber3) fruit3Found = true;
					}
				}
			}

			if (
				count == textureNumber1 ||
				count == textureNumber2 ||
				count == textureNumber3
				) {
				DrawTextureV(textures[count], randomPositions[count], WHITE);
			}
			else {
				DrawTextureV(textures[randomNumbers[count]], randomPositions[count], WHITE);
			}

			count++;

		}

	}

	DrawCursor();

	if (timeLeft == 0) screen = FAILURE;
	if (gameSuccess) screen = SUCCESS;


}

void Game::SuccessScreen() {

}

void Game::FailureScreen() {

}

void Game::InitTextures() {


	Texture2D* MyTextures = new Texture2D[36];

	for (int i = 0; i < totalTextures; i++) {

		std::ostringstream oss;
		oss << "resources/textures/" << i << ".png";
		std::string routeTexture = oss.str();
		const char* cstr = routeTexture.c_str();
		MyTextures[i] = LoadTexture(cstr);

	}

	textures = MyTextures;
}

Vector2* Game::GetRandomVectors() {

	Vector2* randomVectors = new Vector2[60];

	float startX = 150.f;
	float startY = 200.f;
	static int count = 0;

	for (int i = 0; i < 12; i++) {

		for (int j = 0; j < 5; j++) {

			randomVectors[count] = Vector2{
				(startX + (105 * i) + GetRandomValue(-35, 35)),
				(startY + (105 * j) + GetRandomValue(-35, 5))
			};
			count++;
		}

	}

	return randomVectors;

}

int* Game::GetRandomNumbersExcept(int numbersToAvoid1, int numbersToAvoid2, int numbersToAvoid3) {

	int* randomNumbers = new int[totalTextures];

	for (int i = 0; i < totalTextures; i++) {

		randomNumbers[i] = GetRandomValue(0, (totalTextures - 1));

		// No quiero las soluciones
		while (
			randomNumbers[i] == numbersToAvoid1 ||
			randomNumbers[i] == numbersToAvoid2 ||
			randomNumbers[i] == numbersToAvoid3
			) {
			randomNumbers[i] = GetRandomValue(0, (totalTextures - 1));
		}
	}

	return randomNumbers;

}

void Game::DrawCursor() {

	cursorPosition = GetMousePosition();

	Cursor1 = Rectangle{ cursorPosition.x, cursorPosition.y, 4.f, 30.f };
	DrawRectanglePro(Cursor1, Vector2{ 2.f, 15.f }, 0, WHITE);

	Cursor2 = Rectangle{ cursorPosition.x, cursorPosition.y, 30.f, 4.f };
	DrawRectanglePro(Cursor2, Vector2{ 15.f, 2.f }, 0, WHITE);

}

bool Game::IsHoveringImage(Vector2 pos) {

	return
		CheckCollisionRecs(Cursor1, Rectangle{ pos.x,pos.y, 60.f, 60.f }) ||
		CheckCollisionRecs(Cursor2, Rectangle{ pos.x,pos.y, 60.f, 60.f });

}

bool Game::ClickedOnImage(Vector2 pos) {

	return
		IsMouseButtonPressed(0) &&
		GetMouseX() >= pos.x &&
		GetMouseX() <= (pos.x + 80) &&
		GetMouseY() >= pos.y &&
		GetMouseY() <= (pos.y + 80);

}

void Game::SoundHover(int newNumberHovered) {

	if (numberHovered != newNumberHovered) {
		SetSoundVolume(hoverSound, 0.6f);
		PlaySound(hoverSound);
		numberHovered = newNumberHovered;
	}
}

void Game::SoundClick(int newNumberClicked) {

	if (numberClicked != newNumberClicked) {
		SetSoundVolume(clickSound, 0.6f);
		PlaySound(clickSound);
		numberClicked = newNumberClicked;
	}

}