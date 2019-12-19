#ifndef ENEMY_H_
#define ENEMY_H_

#define ENEMY_NUM  26
#define ENEMY_SNUM 50

#include "main.h"

struct ENEMY {
	//座標
	D3DXVECTOR2 enemy_position;

	//画像サイズ
	float width, height;

	//出現、停止、帰還、発射タイミング
	int in_time, stop_time, shot_time, out_time;
	
	//敵の種類
	int type;
	//弾の種類
	int shotType;
	//移動パターン
	int move_pattern;
	//ショットパターン
	int shot_pattern;

	//Health Point
	int hp;
	//持っているアイテムタイプ
	int item;

	//敵が出現してからのカウン
	int count;

	//Control how many bullets an enemy shoots once
	int bulletNum;
	//Shooting bullet's radian
	double shootingRadian;

	//敵消滅フラグ
	bool deadFlag;
	//敵クラス消滅フラグ
	bool endFlag;

	//弾構造体
	E_SHOT shot[ENEMY_SNUM];
	//ショットが撃てるようになったかのフラグ
	bool sFlag;
	//ショットが打てるようになってからのカウント
	int sCount;
};

struct ENEMYDATA {
	int type;				//敵種類
	int shotType;			//弾種類
	int move_pattern;		//移動パターン
	int shot_pattern;		//発射パターン
	int in_time;			//出現時間
	int stop_time;			//停止時間
	int shot_time;			//弾発射時間
	int out_time;			//帰還時間
	int x;					//x座標
	int y;					//y座標
	int speed;				//弾スピード
	int hp;					//HP
	int item;				//アイテム
};

void enemyInit();
void enemyUpdate();
void enemyDraw();
void enemyMove();
void enemyShot();
void readENEMYDATA();
bool checkOutOfRange(ENEMY enemy);
bool checkEnemyShotOutOfRange(ENEMY enemy, int index);
void getEnemyPosition(int index, float *x, float *y);
bool getEnemyPositions(int index, float *x, float *y);
bool getEnemyShotPosition(int enemyIndex, int shotIndex, float *x, float *y);
void setEnemyShotFlag(int enemyIndex, int shotIndex, bool flag);
int  getEnemyShotType(int enemyIndex);
void setEnemyDeadFlag(int index);
bool getEnemyDeadFlag(int index);
int  getEnemyInTime(int index);
int  getEnemyItemType(int index);
void setEnemyHp(int index, int num);
int  getEnemyHP(int index);
int  getEnemyLeft();
void setEnemyLeft(int num);

#endif // !ENEMY_H_


