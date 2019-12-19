#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "explosion.h"
#include "player.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"

EXPLOSION explosions[EXPLOSION_MAX];

void explosionInit() {
	//フラグの初期化(未使用)、テクスチャの準備
	Texture_SetLoadFile("Assets/Textures/enemy-death.png", 670, 96);
	Texture_Load();

	for (int i = 0; i < EXPLOSION_MAX; i++) {
		explosions[i].isUse = false;
		explosions[i].textureID = Texture_GetID("Assets/Textures/enemy-death.png");
	}
}

void explosionUninit() {

}

void explosionUpdate() {
	for (int i = 0; i < EXPLOSION_MAX; i++) {
		if (explosions[i].isUse) {
			++explosions[i].counter;
		}
	}
}

void explosionDraw() {
	for (int i = 0; i < EXPLOSION_MAX; i++) {
		if (explosions[i].isUse) {
			int patternX = explosions[i].counter / 5 % 5;
			int patternY = 0;
			Sprite_Draw(explosions[i].textureID, explosions[i].position.x, explosions[i].position.y, patternX * 134, patternY * 96, 134, 96);

			if (patternX == 4) {
				explosions[i].isUse = false;
				explosions[i].counter = 0;
			}
		}
	}
}

void CreateExplosion(float x, float y) {
	for (int i = 0; i < EXPLOSION_MAX; i++) {
		if (!explosions[i].isUse) {
			explosions[i].isUse = true;
			explosions[i].position.x = x;
			explosions[i].position.y = y;
			explosions[i].counter = -1;
			break;
		}
	}
}