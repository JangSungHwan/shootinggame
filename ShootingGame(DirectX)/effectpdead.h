#ifndef EFFECTPDEAD_H_
#define EFFECTPDEAD_H_

#define EFFECT_PDEADNUM 20

struct PDEFFECT_EXPAND {
	float x, y;
	float radian;
	int speed;
};

void pDeadEffectInit();
void pDeadEffectUpdate();
void pDeadEffectDraw();
void pDeadEffectSetFlag(float x, float y);
bool pDeadEffectGetFlag();

#endif // !EFFECTPDEAD_H_

