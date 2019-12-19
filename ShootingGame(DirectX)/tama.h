#ifndef TAMA_H_
#define TAMA_H_

struct TAMA {
	float x, y;
	int tama_textureID;
	bool toggle;		//一時フラグ
	float raise;		//sin波に使う角度の増加量
	double angle;		//角度
};

void  tamaInit();
void  tamaAll(float nowX, float nowY);
float tamaGetPosition();
void  tamaMove(float moveX, float moveY);
void  tamaDraw();

//class TAMA {
//public:
//	TAMA();
//	void All(float nowX, float nowY);
//	float getPosition();
//
//private:
//	float x, y;
//	int tama_textureID;
//	bool toggle;		//一時フラグ
//	int raise;			//sin波に使う角度の増加量
//	double angle;		//角度
//	void Move(float moveX, float moveY);
//	void Draw();
//}

#endif // !TAMA_H_

