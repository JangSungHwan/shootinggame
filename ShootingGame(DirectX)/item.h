#ifndef ITEM_H_
#define ITEM_H_

struct ITEM {
	int type;
	float x, y;
	float prevY;
	double radian;
	bool fall_flag;
	bool flag;
};

void itemInit();
void itemUpdate();
void itemDraw();
void setItemFlag(int index, float x, float y, int type);
bool getItemFlag(int index);
void DestroyItem(int index);
void getItemPosition(int index, float *x, float *y);
int  getItemType(int index);

#endif // !ITEM_H_

