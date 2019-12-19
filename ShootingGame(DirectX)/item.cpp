#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "item.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "mydirect3d.h"

static int g_item_textureID;
ITEM items[ITEM_NUM];

void itemInit() {
	Texture_SetLoadFile("Assets/Textures/items.png", 160, 32);
	Texture_Load();
	g_item_textureID = Texture_GetID("Assets/Textures/items.png");

	for (int i = 0; i < ITEM_NUM; i++) {
		items[i].fall_flag = false;
		items[i].flag = false;
	}
}

void itemUpdate() {
	for (int i = 0; i < ITEM_NUM; i++) {
		float tempY;

		if (!items[i].fall_flag) {
			tempY = items[i].y;
			items[i].y += (items[i].y - items[i].prevY) + 1;
			if ((items[i].y - items[i].prevY) + 1 == 0)
				items[i].fall_flag = true;
			items[i].prevY = tempY;
		}
		else {
			items[i].y += 1.5f;
		}

		if (items[i].y > 850)
			DestroyItem(i);
	}
}

void itemDraw() {
	for (int i = 0; i < ITEM_NUM; i++) {
		if (items[i].flag) {
			Sprite_Draw(g_item_textureID, items[i].x, items[i].y, items[i].type * 32, 0, 32, 32);
		}
	}
}

void setItemFlag(int index, float x, float y, int type) {
	items[index].x = x;
	items[index].y = y - 8;
	items[index].prevY = y;
	items[index].type = type;
	items[index].flag = true;
}

bool getItemFlag(int index) {
	return items[index].flag;
}

void DestroyItem(int index) {
	items[index].fall_flag = false;
	items[index].flag = false;
}

void getItemPosition(int index, float *x, float *y) {
	*x = items[index].x;
	*y = items[index].y;
}

int  getItemType(int index) {
	return items[index].type;
}