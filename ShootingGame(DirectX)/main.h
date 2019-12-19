#ifndef MAIN_H_
#define MAIN_H_

#define SCREEN_WIDTH			1280
#define SCREEN_HEIGHT			900
#define GAME_WIDTH				800
#define GAME_HEIGHT				850

#define PLAYER_INITX			450
#define PLAYER_INITY			800

#define PSHOT_NUM				100
#define PSHOT_SPEED				14

#define TAMA_MAXNUM				4
#define TAMA_INITX				50
#define TAMA_INITY				30
#define TAMA_SHAKE				15

#define BOSS_SHOTNUM			5000
#define BOSS_SHAKE				20

#define ITEM_NUM				100

#define PLAYER_COLLISIONRAD		3
#define ENEMY_COLLISIONRAD		24
#define BOSS_COLLISIONRAD		100
#define ITEM_COLLISIONRAD		16

#define PSHOT_COLLISIONRAD		10
#define ESHOT0_COLLISIONRAD		25
#define ESHOT1_COLLISIONRAD		16
#define ESHOT2_COLLISIONRAD		25

//Enemy(Include boss) bullet struct
struct E_SHOT {
	bool flag;			//弾が発射中かどうか
	double x;			//x座標
	double y;			//y座標
	double radian;		//進む角度
	int width, height;	//画像の幅と高さ
	int pattern;		//ショットパターン
	int speed;			//弾スピード
	int type;			//弾の種類
};

//position       color            texture
#define FVF_VERTEX2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FPS_MEASUREMENT_TIME 1.0

typedef struct Vertex2d_tag {
	// x, y, z, w			座標変換済み頂点の利用　=> RHWに1.0fを設定
	D3DXVECTOR4 position;	//座標, include in d3dx9.h
	//Color
	D3DCOLOR color;
	//uv
	D3DXVECTOR2 uv;			//texture coordinate
}Vertex2d;

static bool Init(HWND hWnd);
static void Uninit(void);
static void Update();
static void Draw(void);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !MAIN_H_