#ifndef TEXTURE_H_
#define TEXTURE_H_

#define FILENAME_MAX		100
#define TEXTUREDATA_MAX		100

typedef struct TextureData_tag {
	char filename[FILENAME_MAX];
	int id;
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;

void Texture_Init();
int Texture_SetLoadFile(const char* pFileName, int width, int height);
int Texture_Load();
void Texture_Release(int ids[], int count);
void Texture_Release();
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);
int Texture_GetWidth(int id);
int Texture_GetHeight(int id);
int Texture_GetID(const char* pFileName);

#endif // !TEXTURE_H
