#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#define EXPLOSION_MAX 100

typedef struct EXPLOSION {
	bool isUse;
	D3DXVECTOR2 position;
	int textureID;
	int counter;
};

void explosionInit();
void explosionUninit();
void explosionUpdate();
void explosionDraw();
void CreateExplosion(float x, float y);

#endif // !EXPLOSION_H_
