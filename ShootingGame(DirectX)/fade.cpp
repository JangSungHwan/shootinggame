#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "fade.h"
#include "main.h"
#include "mydirect3d.h"

FADE_STATE		g_FadeState;
SCENE_STATE		g_FadeNextScene;
float			g_FadeAlpha;
D3DXCOLOR		g_FadeColor;

void fadeInit() {
	g_FadeState = FADE_STATE_NONE;
	g_FadeNextScene = SCENE_NONE;
	g_FadeAlpha = 0.0f;
	g_FadeColor = D3DCOLOR_RGBA(0, 0, 0, 255);
}

void fadeUpdate() {
	if (g_FadeState == FADE_STATE_OUT) {
		if (g_FadeAlpha >= 1.0f) {
			g_FadeAlpha = 1.0f;
			g_FadeState = FADE_STATE_IN;
			SetScene(g_FadeNextScene);
		}

		g_FadeAlpha += 1.0 / 50.0f;
	}
	else if (g_FadeState == FADE_STATE_IN) {
		if (g_FadeAlpha <= 0.0f) {
			g_FadeAlpha = 0.0;
			g_FadeState = FADE_STATE_NONE;
		}

		g_FadeAlpha -= 1.0f / 50.0f;
	}

	D3DXCOLOR color = g_FadeColor;
	color.a = g_FadeAlpha;
	g_FadeColor = color;
}

void fadeDraw() {
	if (g_FadeState == FADE_STATE_NONE)
		return;

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	Vertex2d v[] = {
		{D3DXVECTOR4(		 0.0f,		    0.0f, 0.0f, 1.0f), g_FadeColor, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(SCREEN_WIDTH,		    0.0f, 0.0f, 1.0f), g_FadeColor, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(		 0.0f, SCREEN_HEIGHT, 0.0f, 1.0f), g_FadeColor, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f), g_FadeColor, D3DXVECTOR2(1.0f, 1.0f)},
	};
	
	myDevice->SetFVF(FVF_VERTEX2D);
	myDevice->SetTexture(0, NULL);
	myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
}

void Fade(SCENE_STATE NextScene) {
	g_FadeNextScene = NextScene;
	g_FadeState = FADE_STATE_OUT;
}

FADE_STATE GetFadeState() {
	return g_FadeState;
}