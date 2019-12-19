#include "sprite.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "SpriteAnim.h"

int FrameCount = 0;

void SpriteAnim_Init() {

}

void SpriteAnim_Update() {
	FrameCount++;
}

void SpriteAnim_Draw(int textureID, int playSpeed, float dx, float dy) {

}

void SpriteAnim_Draw(int textureID, int playSpeed, float dx, float dy, float scaleX, float scaleY) {
	//cut_x = pattern % 5 * 140		140x200
	//cut_y = pattern / 5 * 200

	int patternX = FrameCount / playSpeed % 5;
	int patternY = FrameCount / playSpeed / 5;
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200, scaleX, scaleY);
}

void SpriteAnim_Draw_Flip(int textureID, int playSpeed, float dx, float dy, float scaleX, float scaleY) {
	int patternX = 4 - (FrameCount / playSpeed % 5);
	int patternY = 4 - (FrameCount / playSpeed / 5);
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200, scaleX, scaleY);
}

void SpriteAnim_Draw_Rotate(int textureID, int playSpeed, float dx, float dy, float angle, float scaleX, float scaleY) {
	//cut_x = pattern % 5 * 140		140x200
	//cut_y = pattern / 5 * 200

	int patternX = FrameCount / playSpeed % 5;
	int patternY = FrameCount / playSpeed / 5;
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200, dx, dy, angle, scaleX, scaleY);
}

void SpriteAnim_Draw_Flip_Rotate(int textureID, int playSpeed, float dx, float dy, float angle, float scaleX, float scaleY) {
	int patternX = 4 - (FrameCount / playSpeed % 5);
	int patternY = 4 - (FrameCount / playSpeed / 5);
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200, dx, dy, angle, scaleX, scaleY);
}

void SpriteAnim_Uninit(){

}