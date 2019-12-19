#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "effectpdead.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "mydirect3d.h"

static int pDEffectTex1, pDEffectTex2, pDEffectTex3;
float pDEffectX, pDEffectY;
int width[3], height[3];
//Scaling rate
float rate;
//rotation radian
float radian;
//transparency
int alpha;
PDEFFECT_EXPAND expand[EFFECT_PDEADNUM];
int pDEffectCount;
bool pDEffectFlag;

void pDeadEffectInit() {
	Texture_SetLoadFile("Assets/Textures/playereffect1.png", 280, 280);
	Texture_SetLoadFile("Assets/Textures/playereffect2.png", 70, 67);
	Texture_SetLoadFile("Assets/Textures/playereffect3.png", 7, 7);
	Texture_Load();
	pDEffectTex1 = Texture_GetID("Assets/Textures/playereffect1.png");
	pDEffectTex2 = Texture_GetID("Assets/Textures/playereffect2.png");
	pDEffectTex3 = Texture_GetID("Assets/Textures/playereffect3.png");

	rate = 1.f;
	alpha = 255;
	pDEffectCount = 0;
	pDEffectFlag = false;
}

void pDeadEffectUpdate() {
	//Give a radian at the first count
	if (pDEffectCount == 0) {
		for (int i = 0; i < EFFECT_PDEADNUM; i++) {
			expand[i].radian = rand() % 628 / 100;
			expand[i].speed = rand() % 10;
			expand[i].x = pDEffectX;
			expand[i].y = pDEffectY;
		}
	}

	rate = .5f + (pDEffectCount * 0.05f);
	alpha = 255 - (255 / 40) * pDEffectCount;
	radian = 0.05f * pDEffectCount;

	//move position
	for (int i = 0; i < EFFECT_PDEADNUM; i++) {
		expand[i].x += cos(expand[i].radian) * expand[i].speed;
		expand[i].y += sin(expand[i].radian) * expand[i].speed;
	}

	pDEffectCount++;

	if (pDEffectCount == 40) {
		pDEffectFlag = false;
		pDEffectCount = 0;
	}
}

void pDeadEffectDraw() {
	//1st effect
	Sprite_Draw(pDEffectTex1, pDEffectX, pDEffectY, rate, rate, alpha);
	//2nd effect
	Sprite_Draw(pDEffectTex2, pDEffectX, pDEffectY, pDEffectX, pDEffectY, radian, alpha, true);
	Sprite_Draw(pDEffectTex2, pDEffectX, pDEffectY, pDEffectX, pDEffectY, 6.28f - radian, alpha, true);
	//particular effect
	for (int i = 0; i < EFFECT_PDEADNUM; i++) {
		Sprite_Draw(pDEffectTex3, expand[i].x, expand[i].y);
	}
}

void pDeadEffectSetFlag(float x, float y) {
	pDEffectCount = 0;
	pDEffectFlag = true;
	pDEffectX = x;
	pDEffectY = y;
}

bool pDeadEffectGetFlag() {
	return pDEffectFlag;
}