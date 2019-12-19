#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "tama.h"
#include "enemy.h"
#include "player.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "mydirect3d.h"

TAMA tama[TAMA_MAXNUM];

void  tamaInit() {
	for (int i = 0; i < TAMA_MAXNUM; i++) {
		tama[i].x = tama[i].y = 0.f;
		tama[i].tama_textureID = Texture_GetID("Assets/Textures/player.png");
		tama[i].angle = 0;
		tama[i].toggle = false;
		tama[i].raise = 2;
	}
}

void tamaAll(float nowX, float nowY) {
	tamaMove(nowX, nowY);
	tamaDraw();
}

float tamaGetPosition() {
	return tama[0].y;
}

void tamaMove(float nowX, float nowY) {
	for (int i = 0; i < TAMA_MAXNUM; i++) {
		tama[i].angle += tama[i].raise;
		tama[i].x = nowX;
		tama[i].y = nowY + sin(tama[i].angle) * 5;

		if (tama[i].angle == 90)
			tama[i].toggle = true;
		else if(tama[i].angle == -90)
			tama[i].toggle = false;

		if (tama[i].toggle)
			tama[i].raise = -0.05f;
		else
			tama[i].raise = 0.05f;
	}
}

void tamaDraw() {
	for (int i = 0; i < TAMA_MAXNUM; i++) {
		Sprite_Draw(tama[i].tama_textureID, tama[i].x + TAMA_INITX, tama[i].y + TAMA_INITY, 0, 324, 40, 28, tama[i].x + TAMA_INITX, tama[i].y + TAMA_INITY, tama[i].angle);
		Sprite_Draw(tama[i].tama_textureID, tama[i].x - TAMA_INITX, tama[i].y + TAMA_INITY, 0, 324, 40, 28, tama[i].x - TAMA_INITX, tama[i].y + TAMA_INITY, tama[i].angle);
	}
}