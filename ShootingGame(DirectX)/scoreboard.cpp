#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "scoreboard.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "debug_font.h"

static int g_boardTexID, g_numberTexID;
int g_board[3], g_number[10];
int high_score, cur_score, lifeLeft, powerNow;
bool ifGetHighScore;

void scoreBoardInit() {
	Texture_SetLoadFile("Assets/Textures/board.png", 239, 188);
	Texture_SetLoadFile("Assets/Textures/number.png", 400, 40);
	Texture_Load();
	g_boardTexID = Texture_GetID("Assets/Textures/board.png");
	g_numberTexID = Texture_GetID("Assets/Textures/number.png");
	
	cur_score = 0;

	ifGetHighScore = false;
}

void scoreBoardUpdate() {
	if (cur_score > high_score) {
		high_score = cur_score;
		ifGetHighScore = true;
	}
}

void scoreBoardDraw() {
	//Draw the board
	for (int i = 0; i < 4; i++) {
		//Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h)
		Sprite_Draw(g_boardTexID, 950, 100 + 100 * i, 0, i * 47, 239, 47);
	}

	//Draw the scores
	for (int i = 0; i < 4; i++) {
		int score;
		switch (i)
		{
		case 0:		//get high score
			score = getScoreData(HIGH_SCORE);
			break;
		case 1:		//get current score
			score = getScoreData(CURRENT_SCORE);
			break;
		case 2:		//get lifeleft
			score = getScoreData(LIFE_SCORE);
			break;
		case 3:		//get power
			score = getScoreData(POWER_SCORE);
			break;
		default: break;
		}

		if (score == 0)
			Sprite_Draw(g_numberTexID, 850, 150 + 100 * i, 0, 0, 40, 40);
		else {
			int scores[10], ketaNum = 0;
			while (score != 0) {
				scores[ketaNum++] = score % 10;
				score /= 10;
			}

			for (int j = ketaNum - 1; j >= 0; j--) {
				Sprite_Draw(g_numberTexID, 850 + 40 * (ketaNum - j - 1), 150 + 100 * i, scores[j] * 40, 0, 40, 40);
			}
		}
	}
}

void setScoreData(SCOREDATATYPE type, int val) {
	switch (type)
	{
	case HIGH_SCORE:
		high_score += val;
		break;
	case CURRENT_SCORE:
		cur_score += val;
		break;
	case LIFE_SCORE:
		lifeLeft = val;
		break;
	case POWER_SCORE:
		powerNow = val;
		break;
	default: break;
	}
}

int  getScoreData(SCOREDATATYPE type) {
	switch (type)
	{
	case HIGH_SCORE:
		return high_score;
		break;
	case CURRENT_SCORE:
		return cur_score;
		break;
	case LIFE_SCORE:
		return lifeLeft;
		break;
	case POWER_SCORE:
		return powerNow;
		break;
	default: break;
	}

	return -1;
}

int  getScore() {
	return cur_score;
}

int  getHighScore() {
	return high_score;
}

void setHighScore(int score) {
	high_score = score;
}

bool getHighScoreOrNot() {
	return ifGetHighScore;
}
