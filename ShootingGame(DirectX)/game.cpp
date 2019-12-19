#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "game.h"
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "boss.h"
#include "fade.h"
#include "collision.h"
#include "explosion.h"
#include "scoreboard.h"
#include "effectpdead.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "system_timer.h"
#include "debug_font.h"

int g_FrameCount;				//フレームカウンター
int g_FPSBaseFrameCount;		//FPS計測用フレームカウンター
double g_FPSBaseTime;			//FPS計測用時間
float g_FPS;					//FPS

static int g_BG_textureID;
static int g_gameBG_textureID;
static int easyGlowTexID, hardGlowTexID, lunaGlowTexID;

static GAME_DIFFICULTY g_game_difficulty;
float gameBG1Y, gameBG2Y;
int beforeBossCounter, afterBossCounter;
bool kamokuRyunen;

void gameInit() {
	//==============================================System Initialization===============================================//
	DebugFont_Initialize();

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0;

	Texture_SetLoadFile("Assets/Textures/background2.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	Texture_SetLoadFile("Assets/Textures/gameBG.png", GAME_WIDTH, GAME_HEIGHT);
	Texture_SetLoadFile("Assets/Textures/easy_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/hard_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/lunatic_glow.png", 470, 162);
	Texture_Load();
	g_BG_textureID = Texture_GetID("Assets/Textures/background2.png");
	g_gameBG_textureID = Texture_GetID("Assets/Textures/gameBG.png");
	easyGlowTexID = Texture_GetID("Assets/Textures/easy_glow.png");
	hardGlowTexID = Texture_GetID("Assets/Textures/hard_glow.png");
	lunaGlowTexID = Texture_GetID("Assets/Textures/lunatic_glow.png");

	gameBG1Y = SCREEN_HEIGHT / 2.f;
	gameBG2Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);

	beforeBossCounter = 0;
	afterBossCounter = 0;

	kamokuRyunen = false;
	//==================================================================================================================//

	playerInit();
	enemyInit();
	bossInit();
	itemInit();
	scoreBoardInit();
	pDeadEffectInit();
	explosionInit();

	if (getGameDifficulty() != DIFFICULTY_LUNATIC)
		PlaySound(SOUND_LABEL_STAGEBGM);
}

void gameUninit() {
	if (kamokuRyunen && !getBossAppearFlag())
		StopSound(SOUND_LABEL_STAGEBGM);
	else if (kamokuRyunen && getBossAppearFlag()) {
		StopSound(SOUND_LABEL_BOSSBGM);
		StopSound(SOUND_LABEL_BOSSLUNABGM);
	}
	else if (!kamokuRyunen) {
		StopSound(SOUND_LABEL_BOSSBGM);
		StopSound(SOUND_LABEL_BOSSLUNABGM);
	}
}

void gameUpdate() {

	g_FrameCount++;
	double time = SystemTimer_GetTime();
	float timeDelta = time - g_FPSBaseTime;

	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}

	gameBG1Y += 5.f;
	gameBG2Y += 5.f;
	if (gameBG1Y >= (SCREEN_HEIGHT / 2.f) + Texture_GetWidth(g_gameBG_textureID))
		gameBG1Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);
	if (gameBG2Y >= (SCREEN_HEIGHT / 2.f) + Texture_GetWidth(g_gameBG_textureID))
		gameBG2Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);

	itemUpdate();

	playerUpdate();

	if (getGameDifficulty() != DIFFICULTY_LUNATIC)
		enemyUpdate();

	scoreBoardUpdate();

	if (getBossAppearFlag())
		bossMove();

	//プレイヤー死亡エフェクト
	if (pDeadEffectGetFlag())
		pDeadEffectUpdate();

	explosionUpdate();

	//当たり判定
	checkCollisionAll();
	checkBossCollision();

	//エネミー数チェック、0ならボスカウンターがカウントスタート
	if (getEnemyLeft() == 0) {
		beforeBossCounter++;
	}
	if (beforeBossCounter == 400) {
		setBossAppearFlag(true);
		StopSound(SOUND_LABEL_STAGEBGM);

		if (getGameDifficulty() != DIFFICULTY_LUNATIC)
			PlaySound(SOUND_LABEL_BOSSBGM);
		else
			PlaySound(SOUND_LABEL_BOSSLUNABGM);
	}

	//Clear the Stage
	if (!getBossFlag()) {
		afterBossCounter++;

		if (afterBossCounter == 400) {
			Fade(SCENE_RESULT);
		}
	}
	//Lost all the lives
	if (getPlayerLife() == 0) {
		afterBossCounter++;

		if (afterBossCounter == 400) {
			kamokuRyunen = true;
			Fade(SCENE_RESULT);
		}
	}
}

void gameDraw() {

	Sprite_Draw(g_gameBG_textureID, 50.f + GAME_WIDTH / 2.f, gameBG1Y);
	Sprite_Draw(g_gameBG_textureID, 50.f + GAME_WIDTH / 2.f, gameBG2Y);

	itemDraw();
	playerDraw();
	enemyDraw();

	if (getBossAppearFlag())
		bossDraw();

	if (pDeadEffectGetFlag())
		pDeadEffectDraw();

	explosionDraw();

	Sprite_Draw(g_BG_textureID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

	scoreBoardDraw();

	if (g_game_difficulty == DIFFICULTY_EASY) {
		Sprite_Draw(easyGlowTexID, 1050, 40, 0.3f, 0.3f);
	}
	else if (g_game_difficulty == DIFFICULTY_HARD) {
		Sprite_Draw(hardGlowTexID, 1050, 40, 0.3f, 0.3f);
	}
	else if (g_game_difficulty == DIFFICULTY_LUNATIC) {
		Sprite_Draw(lunaGlowTexID, 1050, 40, 0.3f, 0.3f);
	}
}

void setGameDifficulty(GAME_DIFFICULTY difficulty) {
	g_game_difficulty = difficulty;
}

GAME_DIFFICULTY getGameDifficulty() {
	return g_game_difficulty;
}

bool RyunenOrNot() {
	return kamokuRyunen;
}