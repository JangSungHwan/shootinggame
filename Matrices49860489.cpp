// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  1923
#define SCREEN_HEIGHT 1080
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ENEMY_NUM 20
#define BULLET_NUM 100

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")





// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface


LPDIRECT3DTEXTURE9 gameoverscreen;
LPDIRECT3DTEXTURE9 background;						// sprite declarations
LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_hero;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite



									 // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);
bool square_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;


enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };


//기본 클래스 
class entity {

public:
	float x_pos;
	float y_pos;
	int status;
	int HP;

};


bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if ((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1))
		return true;
	else
		return false;

}

bool square_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{

	if ((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1))
		return true;
	else
		return false;

}

//주인공 클래스 
class Hero :public entity {
public:
	bool isSuper;
	int superCount;
public:
	void fire();
	void super_fire();
	void move(int i);
	void init(float x, float y);


};

void Hero::super_fire()
{
	isSuper = true;
}

void Hero::init(float x, float y)
{
	isSuper = false;
	superCount = 0;

	x_pos = x;
	y_pos = y;

}

void Hero::move(int i)
{
	switch (i)
	{
	case MOVE_UP:
		y_pos -= 10;
		break;

	case MOVE_DOWN:
		y_pos += 10;
		break;


	case MOVE_LEFT:
		x_pos -= 10;
		break;


	case MOVE_RIGHT:
		x_pos += 10;
		break;

	}

}




// 적 클래스 
class Enemy :public entity {

public:



	void fire();
	void init(float x, float y);
	void move();
	bool check_collision(float x, float y);


};

void Enemy::init(float x, float y)
{

	x_pos = x;
	y_pos = y;

}


void Enemy::move()
{
	y_pos += 2;

}

bool Enemy::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (square_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{

		return true;

	}
	else {

		return false;
	}
}





// 총알 클래스 
class Bullet :public entity {

public:
	bool bShow;

	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);


};


bool Bullet::check_collision(float x, float y)
{

	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		bShow = false;
		return true;

	}
	else {

		return false;
	}
}




void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;

}



bool Bullet::show()
{
	return bShow;

}


void Bullet::active()
{
	bShow = true;

}



void Bullet::move()
{
	y_pos -= 20;
}

void Bullet::hide()
{
	bShow = false;

}


//객체 생성 
Hero hero;
Enemy enemy[ENEMY_NUM];
Bullet bullet[BULLET_NUM];

int gameover = 0;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);


	//게임 오브젝트 초기화 
	init_game();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		
			

		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do_game_logic();

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		


		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"gameover.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&gameoverscreen);


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"background.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&background);

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"Panel3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"hero.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite





	return;
}


void init_game(void)
{
	//객체 초기화 
	hero.init(970, 900);

	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; i++)
	{

		enemy[i].init((float)(630+(rand() % 600)), rand() % 200 - 300);
	}

	//총알 초기화 
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].init(hero.x_pos, hero.y_pos);
	}
}


int bullet_count = 0;

void do_game_logic(void)
{

		//주인공 처리 
		if (KEY_DOWN(VK_UP))
			hero.move(MOVE_UP);
	

		if (KEY_DOWN(VK_DOWN))
			hero.move(MOVE_DOWN);
	

		if (KEY_DOWN(VK_LEFT))
			hero.move(MOVE_LEFT);
	

		if (KEY_DOWN(VK_RIGHT))
			hero.move(MOVE_RIGHT);
	

	//적들 처리 
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].y_pos > 1080)
			enemy[i].init((float)(630 + (rand() % 600)), rand() % 200 - 300);
		else
			enemy[i].move();
	}


	//총알 처리 
	
	for(bullet_count=0; bullet_count <BULLET_NUM; bullet_count++){
		if (bullet[BULLET_NUM].show() == false){
	
		
		if (KEY_DOWN(VK_CONTROL))
		{
			hero.super_fire();
			for (int i = 0; i < 3; ++i)
			{
				bullet[i].active();
				bullet[i].init((hero.x_pos - 50.0f) + (i*50), hero.y_pos);				
			}
		}

	
		if (KEY_DOWN(VK_SPACE))
		{
	
				bullet[bullet_count].active();
				bullet[bullet_count].init(hero.x_pos, hero.y_pos);
			
		
		}
		}
	}



	
	
	if (hero.isSuper == true)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (bullet[i].show() == true)
			{
				if (bullet[i].y_pos < -70)
					bullet[i].hide();
				else
					bullet[i].move();
			}

			for (int j = 0; j < ENEMY_NUM; j++)
			{
				if (bullet[i].check_collision(enemy[j].x_pos, enemy[j].y_pos) == true)
				{
					enemy[j].init((float)(630 + (rand() % 600)), rand() % 200 - 300);
					hero.superCount += 1;
					if (hero.superCount == 3)
					{
						hero.superCount = 0;
						hero.isSuper = false;
					}
				}
			}
		}
	}
	else
	{
		if (bullet[0].show() == true)
		{
			if (bullet[0].y_pos < -70)
				bullet[0].hide();
			else
				bullet[0].move();


			//충돌 처리
			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (bullet[0].check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
				{
					enemy[i].init((float)(630 + (rand() % 600)), rand() % 200 - 300);

				}
			}

			
			


		}
		
	}
	//적-히어로 충돌처리
	for (int i = 0; i < ENEMY_NUM; i++) {
				if (enemy[i].check_collision(hero.x_pos, hero.y_pos) == true)
				{
					gameover = 1;
				}
			}
}

// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

											 //UI 창 렌더링 


											 /*
											 static int frame = 21;    // start the program on the final frame
											 if(KEY_DOWN(VK_SPACE)) frame=0;     // when the space key is pressed, start at frame 0
											 if(frame < 21) frame++;     // if we aren't on the last frame, go to the next frame

											 // calculate the x-position
											 int xpos = frame * 182 + 1;

											 RECT part;
											 SetRect(&part, xpos, 0, xpos + 181, 128);
											 D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
											 D3DXVECTOR3 position(150.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
											 d3dspt->Draw(sprite, &part, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));
											*/ 
	if (gameover == true)
	{
		RECT part10;
		SetRect(&part10, 0, 0, 1920, 1080);
		D3DXVECTOR3 center10(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position10(0, 0, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(gameoverscreen, &part10, &center10, &position10, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	
	else {
		RECT part0;
		SetRect(&part0, 0, 0, 1920, 1080);
		D3DXVECTOR3 center0(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position0(0, 0, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(background, &part0, &center0, &position0, D3DCOLOR_ARGB(255, 255, 255, 255));



		//주인공 
		RECT part;
		SetRect(&part, 0, 0, 64, 64);
		D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		////총알 
		if (hero.isSuper == true)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (bullet[i].bShow == true)
				{
					RECT part1;
					SetRect(&part1, 0, 0, 64, 64);
					D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
					D3DXVECTOR3 position1(bullet[i].x_pos, bullet[i].y_pos, 0.0f);  
					
					int r =  rand() % 255;
					int b = 100+ rand() % 155;

					
					// position at 50, 50 with no depth
					d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, r, b, 255));
				}
			}
		}
		else
		{
			if (bullet[0].bShow == true)
			{
				RECT part1;
				SetRect(&part1, 0, 0, 64, 64);
				D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
				D3DXVECTOR3 position1(bullet[0].x_pos, bullet[0].y_pos, 0.0f);    // position at 50, 50 with no depth

				int r = rand() % 255;
				int b = 100 + rand() % 155;

				d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, r, b, 255));
			}
		}



		////에네미 
		RECT part2;
		SetRect(&part2, 0, 0, 64, 64);
		D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY_NUM; i++)
		{
			int r = 100 + rand() % 155;
			int g = 100 + rand() % 155;
			int b = 100 + rand() % 155;
			D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, g, 255));
		}
	}


	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();

	return;
}
