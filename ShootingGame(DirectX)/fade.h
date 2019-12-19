#ifndef FADE_H_
#define FADE_H_

#include "scene.h"

enum FADE_STATE {
	FADE_STATE_NONE,
	FADE_STATE_IN,
	FADE_STATE_OUT
};

void fadeInit();
void fadeUpdate();
void fadeDraw();

void Fade(SCENE_STATE NextScene);
FADE_STATE GetFadeState();

#endif // !FADE_H_