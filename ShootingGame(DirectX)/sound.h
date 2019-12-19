//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <Windows.h>

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// Title BGM
	SOUND_LABEL_STAGEBGM,			// Stage BGM
	SOUND_LABEL_BOSSBGM,			// Boss BGM
	SOUND_LABEL_BOSSLUNABGM,		// Boss Lunatic BGM
	SOUND_LABEL_SE_PSHOT,			// プレイヤー弾発射音
	SOUND_LABEL_SE_ESHOT,			// エネミー弾発射音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音
	SOUND_LABEL_SE_OK,				// OK音
	SOUND_LABEL_SE_SELECT,			// 選択音
	SOUND_LABEL_SE_CANCEL,			// キャンセル音
	SOUND_LABEL_SE_GETITEM,			// Get Item
	SOUND_LABEL_SE_DEATH,			// 死亡音
	SOUND_LABEL_SE_BOSSDEATH,		// Boss 死亡音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
