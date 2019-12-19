#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "main.h"

static LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = nullptr;
static D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);

void Sprite_Init() {
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//Video Ramに空間を確保して
	pDevice->CreateVertexBuffer(sizeof(Vertex2d)*4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_VertexBuffer, NULL);
}

void Sprite_Uninit() {
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

void Sprite_Draw(int textureID, float dx, float dy) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f)},
	};
	/*pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);*/

	//===================================================================//
	Sprite_Init();
	Vertex2d* pV;															 //===================================================================//
	g_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);				 // VertexBuffer只需要被括號包起來的部分就行了                          //
	memcpy(pV, v, sizeof(v));												 // 用IndexBuffer的話也需要用到VertexBuffer                            //
	g_VertexBuffer->Unlock();												 // IndexBuffer因為最快通常用在畫3D不會動的物件上，比如說背景             //
	pDevice->SetStreamSource(0, g_VertexBuffer, 0, sizeof(Vertex2d));		 //===================================================================//

	/*pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetTexture(0, NULL);*/
	//===================================================================//

	LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
	pDevice->CreateIndexBuffer(sizeof(WORD)*6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	WORD* pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	pIndex[0] = 0; pIndex[1] = 1; pIndex[2] = 2; pIndex[3] = 1; pIndex[4] = 3; pIndex[5] = 2;
	g_pIndexBuffer->Unlock();
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	pDevice->SetTexture(0, NULL);
}

//No Cut, Rotating and Alpha Method
void Sprite_Draw(int textureID, float dx, float dy, float centerX, float centerY, float angle, int alpha, bool dummyFlag) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);

	D3DCOLOR tmp_Color = D3DCOLOR_RGBA(255, 255, 255, alpha);

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - width / 2, dy - height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + width / 2, dy - height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - width / 2, dy + height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + width / 2, dy + height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(1.0f, 1.0f)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxR, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixRotationZ(&mtxR, angle);

	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -centerX, -centerY, 0.0f);
	D3DXMatrixTranslation(&mtxIT, centerX, centerY, 0.0f);

	mtxW = mtxT * mtxR * mtxIT;
	//座標変換する
	//D3DXVec4Transform(&v[0].position, &v[0].position, &mtrxR);
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//No Cut, Scaling Method
void Sprite_Draw(int textureID, float dx, float dy, float scaleX, float scaleY) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxS, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixScaling(&mtxS, scaleX, scaleY, 1.0f);
	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -dx, -dy, 0.0f);
	D3DXMatrixTranslation(&mtxIT, dx, dy, 0.0f);

	mtxW = mtxT * mtxS * mtxIT;

	//座標変換する
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//No Cut, Scaling and Alpha Method
void Sprite_Draw(int textureID, float dx, float dy, float scaleX, float scaleY, int alpha) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);

	D3DCOLOR tmp_Color = D3DCOLOR_RGBA(255, 255, 255, alpha);

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - width / 2, dy - height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + width / 2, dy - height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - width / 2, dy + height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + width / 2, dy + height / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(1.0f, 1.0f)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxS, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixScaling(&mtxS, scaleX, scaleY, 1.0f);
	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -dx, -dy, 0.0f);
	D3DXMatrixTranslation(&mtxIT, dx, dy, 0.0f);

	mtxW = mtxT * mtxS * mtxIT;

	//座標変換する
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//No Cut, Rotating + Scaling Method
void Sprite_Draw(int textureID, float dx, float dy, float centerX, float centerY, float angle, float scaleX, float scaleY, float scaleZ) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f)},
		{D3DXVECTOR4(dx + width / 2, dy - height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f)},
		{D3DXVECTOR4(dx - width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f)},
		{D3DXVECTOR4(dx + width / 2, dy + height / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxR, mtxS, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixRotationZ(&mtxR, angle);
	D3DXMatrixScaling(&mtxS, scaleX, scaleY, scaleZ);

	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -centerX, -centerY, 0.0f);
	D3DXMatrixTranslation(&mtxIT, centerX, centerY, 0.0f);

	mtxW = mtxT * mtxR * mtxS * mtxIT;
	//座標変換する
	//D3DXVec4Transform(&v[0].position, &v[0].position, &mtrxR);
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//================================================Cut Method================================================//
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	float u0 = cut_x / (float)width;
	float v0 = cut_y / (float)height;
	float u1 = (cut_x + cut_w) / (float)width;
	float v1 = (cut_y + cut_h) / (float)height;

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//Cut, Rotating Method
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float centerX, float centerY, float angle) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	float u0 = cut_x / (float)width;
	float v0 = cut_y / (float)height;
	float u1 = (cut_x + cut_w) / (float)width;
	float v1 = (cut_y + cut_h) / (float)height;

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	
	//変数宣言
	D3DXMATRIX mtxR, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixRotationZ(&mtxR, angle);

	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -centerX, -centerY, 0.0f);
	D3DXMatrixTranslation(&mtxIT, centerX, centerY, 0.0f);

	//Scale Matrix関数　D3DXMatrixScaling(&mtxS, x scale, y scale, z scale); 1.0 -> 普通

	mtxW = mtxT * mtxR * mtxIT;
	//座標変換する
	//D3DXVec4Transform(&v[0].position, &v[0].position, &mtrxR);
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//Cut, Scaling Method
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float scaleX, float scaleY) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	float u0 = cut_x / (float)width;
	float v0 = cut_y / (float)height;
	float u1 = (cut_x + cut_w) / (float)width;
	float v1 = (cut_y + cut_h) / (float)height;

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxS, mtxT, mtxIT, mtxW;;
	//変数に関数を使用して値を代入する
	D3DXMatrixScaling(&mtxS, scaleX, scaleY, 1.0f);

	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -dx, -dy, 0.0f);
	D3DXMatrixTranslation(&mtxIT, dy, dy, 0.0f);

	mtxW = mtxT * mtxS * mtxIT;

	//座標変換する
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

//Cut, Rotating + Scaling Method
void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float centerX, float centerY, float angle, float scaleX, float scaleY) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	float u0 = cut_x / (float)width;
	float v0 = cut_y / (float)height;
	float u1 = (cut_x + cut_w) / (float)width;
	float v1 = (cut_y + cut_h) / (float)height;

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//変数宣言
	D3DXMATRIX mtxR, mtxS, mtxT, mtxIT, mtxW;
	//変数に関数を使用して値を代入する
	D3DXMatrixRotationZ(&mtxR, angle);
	D3DXMatrixScaling(&mtxS, scaleX, scaleY, 1.0f);

	//平移矩陣
	D3DXMatrixTranslation(&mtxT, -centerX, -centerY, 0.0f);
	D3DXMatrixTranslation(&mtxIT, centerX, centerY, 0.0f);

	mtxW = mtxT * mtxR * mtxS * mtxIT;
	//座標変換する
	//					出力				入力
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].position, &v[i].position, &mtxW);
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}

void Sprite_Draw(int textureID, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, int alpha) {
	int width = Texture_GetWidth(textureID);
	int height = Texture_GetHeight(textureID);
	float u0 = cut_x / (float)width;
	float v0 = cut_y / (float)height;
	float u1 = (cut_x + cut_w) / (float)width;
	float v1 = (cut_y + cut_h) / (float)height;

	D3DCOLOR tmp_Color = D3DCOLOR_RGBA(255, 255, 255, alpha);

	Vertex2d v[] = {
		{D3DXVECTOR4(dx - cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(u0, v0)},
		{D3DXVECTOR4(dx + cut_w / 2, dy - cut_h / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(u1, v0)},
		{D3DXVECTOR4(dx - cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(u0, v1)},
		{D3DXVECTOR4(dx + cut_w / 2, dy + cut_h / 2, 0.0f, 1.0f), tmp_Color, D3DXVECTOR2(u1, v1)},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2d));
	pDevice->SetTexture(0, NULL);
}