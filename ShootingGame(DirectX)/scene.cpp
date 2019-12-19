#include "scene.h"
#include "title.h"
#include "result.h"
#include "game.h"
#include "scoreboard.h"

SCENE_STATE Scene_Now;
int			g_HighScore;
bool		lunaModeLocked;

void sceneInit() {
	Scene_Now = SCENE_NONE;
	g_HighScore = 0;
	lunaModeLocked = true;
}

void SetScene(SCENE_STATE g_Scene) {
	switch (Scene_Now)
	{
	case(SCENE_NONE):
		break;
	case(SCENE_TITLE):
		titleUninit();
		break;
	case(SCENE_GAME):
		g_HighScore = getHighScore();
		lunaModeLocked = getLunaLock();
		gameUninit();
		break;
	case(SCENE_RESULT):
		resultUninit();
		break;
	}

	Scene_Now = g_Scene;

	switch (Scene_Now)
	{
	case(SCENE_NONE):
		break;
	case(SCENE_TITLE):
		setLunaLock(lunaModeLocked);
		titleInit();
		break;
	case(SCENE_GAME):
		setHighScore(g_HighScore);
		gameInit();
		break;
	case(SCENE_RESULT):
		resultInit();
		break;
	}
}

SCENE_STATE getSceneState() {
	return Scene_Now;
}