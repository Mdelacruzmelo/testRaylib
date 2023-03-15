#pragma once
#include "Game.h"
#include "raylib.h"
#include <string>
#include <sstream>

Game::Game() {
	hoverSound = LoadSound("resources/sounds/Hover.wav");
	clickSound = LoadSound("resources/sounds/Click.wav");
	failureSound = LoadSound("resources/sounds/Failure.wav");
	successSound = LoadSound("resources/sounds/Success.wav");
}

void Game::TitleScreen() {

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
	DrawText("Ejercicio Raylib 02", 150, 180, 36, WHITE);
	DrawText("Pulsa ENTER para empezar", 150, 230, 20, GREEN);

	currentLevel = 0;
	timeAccummulated = 0;

	static SaveData myTitleData = LoadData();

	if (!dataLoaded) {
		DrawText("Pulsa ENTER para empezar", 450, 430, 20, GREEN);
		myTitleData = LoadData();
		dataLoaded = true;
	}

	if (myTitleData.length > 0) {
		int* prevRecords = GetRecords(myTitleData.text);
		DrawText("Mejores puntuaciones", 650, 440, 30, WHITE);
		for (int i = 0; i < 3; i++) {
			DrawText(TextFormat("%d", prevRecords[i]), 840, 480 + (50 * i), 36, GOLD);
		}
	}

	if (IsKeyPressed(KEY_ENTER)) screen = LOADING;

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
	DrawRectangleLines(180, 150, 80, 80, ORANGE);
	DrawText("?", 200, 160, 50, GOLD);
	DrawRectangleLines(265, 150, 80, 80, ORANGE);
	DrawText("?", 285, 160, 50, GOLD);
	DrawRectangleLines(350, 150, 80, 80, ORANGE);
	DrawText("?", 370, 160, 50, GOLD);

	DrawText("Si los encuenras antes de que acabe el tiempo", 150, 300, 24, WHITE);
	DrawText("ganaras tiempo para el siguiente nivel", 150, 335, 24, WHITE);
	DrawText("+10s", 180, 370, 40, GREEN);

	DrawText("Marca una puntuacion insuperable al terminar los 3 niveles", 150, 490, 24, WHITE);
	DrawText("12000", 180, 520, 40, YELLOW);

	loadingBarPercentage = (int)((BarWidth * 100) / GetScreenWidth());
	if (IsKeyPressed(KEY_ENTER) || loadingBarPercentage >= 100)
	{
		Reinitialize();
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
	float GrowGaminAlpha = timeGaming / (GAME_TIME + timeAccummulated);
	float GameBarWidth = GetScreenWidth() * GrowGaminAlpha;

	// Si no casteo a int, salen números super raros
	timeLeft = (GAME_TIME + timeAccummulated) - (int)(GrowGaminAlpha * (GAME_TIME + timeAccummulated));

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
	static bool fruit1Found;
	static bool fruit2Found;
	static bool fruit3Found;

	static int* randomNumbers = GetRandomNumbersExcept(textureNumber1, textureNumber2, textureNumber3);

	if (gameReinitialized) {

		textureNumber1 = GetRandomValue(0, (totalTextures - 1));
		textureNumber2 = GetRandomValue(0, (totalTextures - 1));
		textureNumber3 = GetRandomValue(0, (totalTextures - 1));
		fruit1Found = false;
		fruit2Found = false;
		fruit3Found = false;

		gameReinitialized = false;

		while (textureNumber1 == textureNumber2) {
			textureNumber2 = GetRandomValue(0, (totalTextures - 1));
		}
		while (textureNumber3 == textureNumber1 || textureNumber3 == textureNumber2) {
			textureNumber3 = GetRandomValue(0, (totalTextures - 1));
		}

		randomNumbers = GetRandomNumbersExcept(textureNumber1, textureNumber2, textureNumber3);
	}

	gameSuccess = fruit1Found && fruit2Found && fruit3Found;



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
				// Draw help
				// DrawRectangleLines((int)randomPositions[count].x, (int)randomPositions[count].y, 60, 60, ORANGE);
				DrawTextureV(textures[count], randomPositions[count], WHITE);
			}
			else {
				DrawTextureV(textures[randomNumbers[count]], randomPositions[count], WHITE);
			}

			count++;
		}
	}

	DrawCursor();

	if (timeLeft == 0) {
		Reinitialize();
		screen = FAILURE;
	}
	if (gameSuccess) screen = SUCCESS;
}

void Game::SuccessScreen() {

	PlaySuccessSound();

	int recordNumber = timeLeft * 1000;
	static int* pointsPerLevel = new int[3]; // Por si a futuro quiero guardar por nivel
	pointsPerLevel[currentLevel] = recordNumber;

	if (!timeAdded) {
		timeAccummulated += timeLeft;
		timeAdded = true;
	}

	if (currentLevel == 2 && !gameSaved) {

		for (int i = 0; i < 3; i++) {
			recordAccumulated += pointsPerLevel[i];
		}

		std::string strRecord = std::to_string(recordAccumulated);
		const int recordStrLength = strRecord.length();
		char* record = new char[recordStrLength + 1]; // \0  for the end of char*
		strcpy(record, strRecord.c_str());
		SaveData myData = LoadData();

		if (myData.length > 0) {

			SaveRecord(recordAccumulated, myData.text);
			gameSaved = true;

		}
		else {

			DrawText("Error, could recover data.", 140, 140, 20, WHITE);
			DrawText("Press ENTER to quit game", 140, 200, 20, WHITE);

			if (IsKeyPressed(KEY_ENTER)) CloseWindow();

		}
	}

	if (currentLevel < 2) {

		DrawText("Bien hecho!", 150, 240, 36, WHITE);
		DrawText("acumulaste", 150, 290, 24, WHITE);
		DrawText(TextFormat("+%d segundos", timeAccummulated), 290, 290, 24, GREEN);
		DrawText("Preparate para la siguiente ronda", 150, 350, 24, GOLD);

	}
	else {

		DrawText("Felicidades!", 150, 240, 36, WHITE);
		DrawText(TextFormat("ganaste %d puntos!", recordAccumulated), 150, 290, 24, GOLD);

	}

	static int loadingTime = 3;

	WAITING_AFTER_GAME += GetFrameTime();
	float GrowAlpha = (float)(WAITING_AFTER_GAME / loadingTime);
	float BarWidth = GetScreenWidth() * GrowAlpha;

	DrawRectangle(0, 0, (int)BarWidth, 30, WHITE);

	if ((BarWidth * 100) / GetScreenWidth() >= 100)
	{
		Reinitialize();
		currentLevel += 1;

		if (currentLevel < 3) screen = GAME;
		else screen = TITLE;
	}

}

void Game::FailureScreen() {

	PlayFailureSound();

	DrawText("Que pena", 150, 240, 36, WHITE);
	DrawText("no sirves para esto :(", 150, 290, 24, WHITE);

	static int failureTime = 5;

	WAITING_AFTER_GAME += GetFrameTime();
	float GrowAlpha = (float)(WAITING_AFTER_GAME / failureTime);
	float BarWidth = GetScreenWidth() * GrowAlpha;

	DrawRectangle(0, 0, (int)BarWidth, 30, WHITE);

	if ((BarWidth * 100) / GetScreenWidth() >= 100)
	{
		currentLevel = 0;
		Reinitialize();
		screen = TITLE;
	}

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

SaveData Game::LoadData() {

	// Previous saved data (char previous record to int length)
	char* savedData = LoadFileText("resources/savings/main.txt");
	std::string strSaveData = std::to_string(sizeof(savedData));
	const unsigned int lengthSavings = strSaveData.length();

	return SaveData{
		lengthSavings,
		savedData
	};

}

void Game::SaveRecord(int recordNumber, char* savedData) {

	// Read previous data

	int* results = new int[3];
	int* resultsLengths = new int[3];
	const char* delimiter = ",";
	int* count = new int[0];

	const char** resultsPointers = TextSplit(savedData, *delimiter, count);

	std::string str1 = resultsPointers[0];
	std::string str2 = resultsPointers[1];
	std::string str3 = resultsPointers[2];

	results[0] = std::stoi(str1);
	results[1] = std::stoi(str2);
	results[2] = std::stoi(str3);

	DrawText(str1.c_str(), 140, 140, 20, WHITE);

	if (recordNumber > results[0]) {
		results[2] = results[1];
		str3 = std::to_string(results[2]);
		results[1] = results[0];
		str2 = std::to_string(results[1]);
		results[0] = recordNumber;
		str1 = std::to_string(results[0]);
	}
	else if (recordNumber > results[1]) {
		results[2] = results[1];
		str3 = std::to_string(results[2]);
		results[1] = recordNumber;
		str2 = std::to_string(results[1]);
	}
	else if (recordNumber > results[2]) {
		results[2] = recordNumber;
		str3 = std::to_string(results[2]);
	}

	resultsLengths[0] = str1.length();
	resultsLengths[1] = str2.length();
	resultsLengths[2] = str3.length();

	char* dataToSave = new char[resultsLengths[0] + resultsLengths[1] + resultsLengths[2] + 1];

	char* record1 = new char[resultsLengths[0]];
	strcpy(record1, str1.c_str());

	char* record2 = new char[resultsLengths[0]];
	strcpy(record2, str2.c_str());

	char* record3 = new char[resultsLengths[0]];
	strcpy(record3, str3.c_str());

	dataToSave = strcat(record1, ",");
	dataToSave = strcat(dataToSave, record2);
	dataToSave = strcat(dataToSave, ",");
	dataToSave = strcat(dataToSave, record3);

	SaveFileText("resources/savings/main.txt", dataToSave);
}

int* Game::GetRecords(char* savedTextData) {

	int* results = new int[3];
	const char* delimiter = ",";
	int* count = new int[0];

	const char** resultsPointers = TextSplit(savedTextData, *delimiter, count);

	std::string str1 = resultsPointers[0];
	std::string str2 = resultsPointers[1];
	std::string str3 = resultsPointers[2];

	results[0] = std::stoi(str1);
	results[1] = std::stoi(str2);
	results[2] = std::stoi(str3);

	return results;

}

void Game::Reinitialize() {

	if (!gameReinitialized) {

		gameSaved = false;
		gameReinitialized = true;
		recordAccumulated = 0;
		timeInLoading = 0;
		timeGaming = 0;
		timeLeft = 0;
		gameSuccess = false;
		WAITING_AFTER_GAME = 0;
		playedResultSound = false;
		timeAdded = false;
		dataLoaded = false;
	}

	gameReinitialized = true;
}

void Game::PlayFailureSound()
{
	if (!playedResultSound) {
		SetSoundVolume(failureSound, 1);
		PlaySound(failureSound);
		playedResultSound = true;
	}
}

void Game::PlaySuccessSound() {

	if (!playedResultSound) {
		SetSoundVolume(successSound, 1);
		PlaySound(successSound);
		playedResultSound = true;
	}

}