#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "result.h"
#include "main.h"
#include "game.h"
#include "scoreboard.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"

static int passTexID, failTexID, highScoreTexID, spaceConTexID, g_numberTexID;
int resultCounter;

void resultInit() {
	Texture_SetLoadFile("Assets/Textures/pass.png", 1280, 900);
	Texture_SetLoadFile("Assets/Textures/fail.png", 1280, 900);
	Texture_SetLoadFile("Assets/Textures/highScore.png", 450, 108);
	Texture_SetLoadFile("Assets/Textures/spaceContinue.png", 620, 88);
	Texture_SetLoadFile("Assets/Textures/number.png", 400, 40);
	Texture_Load();
	passTexID = Texture_GetID("Assets/Textures/pass.png");
	failTexID = Texture_GetID("Assets/Textures/fail.png");
	highScoreTexID = Texture_GetID("Assets/Textures/highScore.png");
	spaceConTexID = Texture_GetID("Assets/Textures/spaceContinue.png");
	g_numberTexID = Texture_GetID("Assets/Textures/number.png");

	resultCounter = 0;

	PlaySound(SOUND_LABEL_TITLEBGM);
}

void resultUninit() {
	StopSound(SOUND_LABEL_TITLEBGM);
}

void resultUpdate() {
	resultCounter++;

	if (Keyboard_IsTrigger(DIK_SPACE)) {	//Press Space
		Fade(SCENE_TITLE);
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
}

void resultDraw() {
	if (RyunenOrNot()) {		//Fail the course
		Sprite_Draw(failTexID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		if ((resultCounter / 20) % 2 == 0)
			Sprite_Draw(spaceConTexID, 950, 800);
	}
	else {						//Clear the stage!!!
		Sprite_Draw(passTexID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		//Show the score
		int score = getScore();
		if (score == 0)
			Sprite_Draw(g_numberTexID, 930, 400, 0, 0, 40, 40);
		else {
			int scores[10], ketaNum = 0;
			while (score != 0) {
				scores[ketaNum++] = score % 10;
				score /= 10;
			}

			for (int j = ketaNum - 1; j >= 0; j--) {
				Sprite_Draw(g_numberTexID, (930 - ketaNum / 2 * 40) + 40 * (ketaNum - j - 1), 400, scores[j] * 40, 0, 40, 40);
			}
		}

		//If get high score
		if (getHighScoreOrNot()) {
			Sprite_Draw(highScoreTexID, 950, 700);
		}

		if ((resultCounter / 20) % 2 == 0)
			Sprite_Draw(spaceConTexID, 950, 800);
	}
}
