#pragma once

enum E_Screen
{
	TITLE,
	GAME,
	SUCCESS,
	FAILURE,
	LOADING,
};

struct SaveData
{
	unsigned int length;
	char* text;
};