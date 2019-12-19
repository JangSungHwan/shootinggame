#pragma once
#ifndef SPRITEANIM_H_
#define SPRITEANIM_H_

#define COCO_WALK_ANIM_PATTERN_MAX	(13)

void SpriteAnim_Init();
void SpriteAnim_Update();
void SpriteAnim_Draw(int textureID, int playSpeed, float dx, float dy);
void SpriteAnim_Draw(int textureID, int playSpeed, float dx, float dy, float scaleX, float scaleY);
void SpriteAnim_Draw_Flip(int textureID, int playSpeed, float dx, float dy, float scaleX, float scaleY);
void SpriteAnim_Draw_Rotate(int textureID, int playSpeed, float dx, float dy, float angle, float scaleX, float scaleY);
void SpriteAnim_Draw_Flip_Rotate(int textureID, int playSpeed, float dx, float dy, float angle, float scaleX, float scaleY);
void SpriteAnim_Uninit();

#endif // !SPRITEANIM_H_

