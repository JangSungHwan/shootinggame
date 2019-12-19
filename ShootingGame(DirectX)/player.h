#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_WIDTH	100
#define PLAYER_HEIGHT	100
#define PLAYER_DEFAULT_SPEED 9.0f

struct SHOT {
	bool flag;				//弾が発射中かどうか
	double x;				//x座標
	double y;				//y座標
	int gh;					//グラフィックハンドル
	int width, height;		//画像の幅と高さ
	double radian;			//進む角度
	int type;				//弾の種類
};

void playerInit();
void playerUninit();
void playerUpdate();
void playerDraw();
void playerMove();
void playerShot();
void getPlayerPosition(float *x, float *y);
bool getPlayerShotPosition(int index, float *x, float *y);
void setPlayerShotFlag(int index, bool flag);
void setPlayerDamageFlag();
bool getPlayerDamageFlag();
bool getPlayerInvincibleFlag();
int  getPlayerLife();
void setPlayerLife();
bool checkShotOutOfRange(int index);
void setPlayerPower(int p);
int  getPlayerPower();
void showTama();
int  searchNearbyEnemy();
void tamaShotSet(int index);

#endif // !PLAYER_H_
