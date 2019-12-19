#ifndef SCENE_H_
#define SCENE_H_

enum SCENE_STATE {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
};

void sceneInit();
void SetScene(SCENE_STATE g_Scene);
SCENE_STATE getSceneState();

#endif // !SCENE_H_