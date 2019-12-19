#pragma once
#ifndef MYDIRECT3D_H
#define MYDIRECT3D_H

bool MyDirect3D_Init(HWND hWnd);
void MyDirect3D_Uninit();
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice();

#endif // !MYDIRECT3D_H