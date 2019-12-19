#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "title.h"
#include "main.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "mydirect3d.h"

static int titleTexID;
static int startTexID, startGlowTexID;
static int exitTexID, exitGlowTexID;
static int easyTexID, easyGlowTexID;
static int hardTexID, hardGlowTexID;
static int lunaTexID, lunaLockTexID, lunaGlowTexID;

//Menu
bool inMenu, difficultyChoose, lunaIsLocked;
int  selected;
int  animCounter;
bool animSelect, animEnter;

//Exit game bool
bool exitGame;

void titleInit() {
	Texture_SetLoadFile("Assets/Textures/title.png", 1280, 900);
	Texture_SetLoadFile("Assets/Textures/start.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/start_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/exit.png", 311, 142);
	Texture_SetLoadFile("Assets/Textures/exit_glow.png", 330, 162);
	Texture_SetLoadFile("Assets/Textures/easy.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/easy_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/hard.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/hard_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/lunatic.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/lunatic_lock.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/lunatic_glow.png", 470, 162);
	Texture_Load();
	titleTexID = Texture_GetID("Assets/Textures/title.png");
	startTexID = Texture_GetID("Assets/Textures/start.png");
	startGlowTexID = Texture_GetID("Assets/Textures/start_glow.png");
	exitTexID = Texture_GetID("Assets/Textures/exit.png");
	exitGlowTexID = Texture_GetID("Assets/Textures/exit_glow.png");
	easyTexID = Texture_GetID("Assets/Textures/easy.png");
	easyGlowTexID = Texture_GetID("Assets/Textures/easy_glow.png");
	hardTexID = Texture_GetID("Assets/Textures/hard.png");
	hardGlowTexID = Texture_GetID("Assets/Textures/hard_glow.png");
	lunaTexID = Texture_GetID("Assets/Textures/lunatic.png");
	lunaLockTexID = Texture_GetID("Assets/Textures/lunatic_lock.png");
	lunaGlowTexID = Texture_GetID("Assets/Textures/lunatic_glow.png");

	inMenu = true;
	difficultyChoose = false;
	selected = 0;
	animCounter = 0;
	animSelect = false;
	animEnter = false;
	exitGame = false;

	PlaySound(SOUND_LABEL_TITLEBGM);
}

void titleUninit() {
	StopSound(SOUND_LABEL_TITLEBGM);
}

void titleUpdate() {

	if (Keyboard_IsTrigger(DIK_UP)) {			//Up
		selected--;

		if (inMenu) {
			if (selected < 0)
				selected = 1;
		}
		else if(difficultyChoose) {
			if (selected < 0 && lunaIsLocked)
				selected = 1;
			else if (selected < 0 && !lunaIsLocked)
				selected = 2;
		}

		animSelect = true;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	if (Keyboard_IsTrigger(DIK_DOWN)) {			//Down
		selected++;

		if (inMenu) {
			if (selected > 1)
				selected = 0;
		}
		else if (difficultyChoose) {
			if (selected > 1 && lunaIsLocked)
				selected = 0;
			else if (selected > 2)
				selected = 0;
		}

		animSelect = true;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	if (Keyboard_IsTrigger(DIK_RETURN)) {		//Press Enter
		PlaySound(SOUND_LABEL_SE_OK);

		if (inMenu) {
			if (selected == 0) {				//Enter choose difficulty phase
				inMenu = false;
				difficultyChoose = true;
			}
			else if (selected == 1) {			//Exit the game
				exitGame = true;
			}
		}
		else if (difficultyChoose) {
			animEnter = true;

			if (selected == 0) {				//Easy Mode
				setGameDifficulty(DIFFICULTY_EASY);
			}
			else if (selected == 1) {			//Hard Mode
				setGameDifficulty(DIFFICULTY_HARD);
			}
			else if (selected == 2) {			//Lunatic Mode
				setGameDifficulty(DIFFICULTY_LUNATIC);
			}
		}
	}
	if (Keyboard_IsTrigger(DIK_ESCAPE)) {		//Press ESC
		if (inMenu) {
			if (selected != 1) {					//Change the selection to exit 
				selected = 1;
				PlaySound(SOUND_LABEL_SE_CANCEL);
			}
			else {									//Exit the game
				exitGame = true;
				PlaySound(SOUND_LABEL_SE_CANCEL);
				Sleep(1000);
			}
		}
		else if (difficultyChoose) {
			difficultyChoose = false;
			inMenu = true;
			selected = 0;
			PlaySound(SOUND_LABEL_SE_CANCEL);
		}
	}

	if (difficultyChoose) {
		if (animEnter) {
			if (animCounter == 21)
				Fade(SCENE_GAME);
		}
	}

	if (animSelect) {
		animCounter++;

		if (animCounter % 4 == 0) {
			animCounter = 0;
			animSelect = false;
		}
	}

	if (animEnter) {
		animCounter++;
	}
}

void titleDraw() {
	Sprite_Draw(titleTexID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

	if (inMenu) {
		if (selected == 0) {
			Sprite_Draw(startGlowTexID, 1050.f - (animCounter % 3) * 10.f, 500.f);
			Sprite_Draw(exitTexID, 1050.f, 700.f);
		}
		else if (selected == 1) {
			Sprite_Draw(startTexID, 1050.f, 500.f);
			Sprite_Draw(exitGlowTexID, 1050.f - (animCounter % 3) * 10.f, 700.f);
		}
	}
	else if (difficultyChoose) {
		if (selected == 0) {
			if (!animEnter) {		//Normal Phase
				Sprite_Draw(easyGlowTexID, 1050.f - (animCounter % 3) * 10.f, 400.f);
				Sprite_Draw(hardTexID, 1050.f, 600.f);

				if (lunaIsLocked)
					Sprite_Draw(lunaLockTexID, 1050.f, 800.f);
				else
					Sprite_Draw(lunaTexID, 1050.f, 800.f);
			}
			else {					//When press enter, do the animation
				if (animCounter % 3 == 0)
					Sprite_Draw(easyGlowTexID, 1050.f, 400.f);
				Sprite_Draw(hardTexID, 1050.f, 600.f);

				if (lunaIsLocked)
					Sprite_Draw(lunaLockTexID, 1050.f, 800.f);
				else
					Sprite_Draw(lunaTexID, 1050.f, 800.f);
			}
		}
		else if (selected == 1) {
			if (!animEnter) {		//Normal Phase
				Sprite_Draw(easyTexID, 1050.f, 400.f);
				Sprite_Draw(hardGlowTexID, 1050.f - (animCounter % 3) * 10.f, 600.f);

				if (lunaIsLocked)
					Sprite_Draw(lunaLockTexID, 1050.f, 800.f);
				else
					Sprite_Draw(lunaTexID, 1050.f, 800.f);
			}
			else {					//When press enter, do the animation
				Sprite_Draw(easyTexID, 1050.f, 400.f);
				if (animCounter % 3 == 0)
					Sprite_Draw(hardGlowTexID, 1050.f, 600.f);

				if (lunaIsLocked)
					Sprite_Draw(lunaLockTexID, 1050.f, 800.f);
				else
					Sprite_Draw(lunaTexID, 1050.f, 800.f);
			}
		}
		else if (selected == 2) {
			if (!animEnter) {		//Normal Phase
				Sprite_Draw(easyTexID, 1050.f, 400.f);
				Sprite_Draw(hardTexID, 1050.f, 600.f);
				Sprite_Draw(lunaGlowTexID, 1050.f - (animCounter % 3) * 10.f, 800.f);
			}
			else {
				Sprite_Draw(easyTexID, 1050.f, 400.f);
				Sprite_Draw(hardTexID, 1050.f, 600.f);
				if (animCounter % 3 == 0)
					Sprite_Draw(lunaGlowTexID, 1050.f - (animCounter % 3) * 10.f, 800.f);
			}
		}
	}
}

bool getExitGameBool() {
	return exitGame;
}

void setLunaLock(bool flag) {
	lunaIsLocked = flag;
}

bool getLunaLock() {
	return lunaIsLocked;
}