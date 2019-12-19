#ifndef BOSS_H_
#define BOSS_H_

#define BOSS_HP 2000
#define BOSS_LUNA_HP 4000

void bossInit();
void bossDraw();
void bossMove();
void bossMoveInit(float nextX, float nextY, int state);
void bossMoveToDefault();
void bossAppear();
void bossMovePattern1();
void bossMovePattern2();
void bossMovePattern3();
void bossMovePattern4();
void setBossMovePattern(int pattern);
void bossShot();
void setBossShotPattern(int pattern);
int  shotSearch();
bool bossShotRangeCheck(int index);

void setBossDamageFlag();
void setBossFlag(bool flag);
bool getBossFlag();
bool getBossInvincibleFlag();
void setBossAppearFlag(bool flag);
bool getBossAppearFlag();

void getBossPosition(float* x, float *y);
bool getBossShotPosition(int index, float *x, float *y, int *type);
void setBossShotFlag(int index, bool flag);

int  setBossHp(int damage);
int  getPrevBossHp();

void setBossDamageSetting();


#endif // !BOSS_H_

