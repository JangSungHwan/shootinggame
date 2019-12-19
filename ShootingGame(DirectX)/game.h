#ifndef GAME_H_
#define GAME_H_

extern int g_FrameCount;

enum GAME_DIFFICULTY {
	DIFFICULTY_EASY,
	DIFFICULTY_HARD,
	DIFFICULTY_LUNATIC
};

void gameInit();
void gameUninit();
void gameUpdate();
void gameDraw();
void setGameDifficulty(GAME_DIFFICULTY difficulty);
GAME_DIFFICULTY getGameDifficulty();
bool RyunenOrNot();

#endif // !GAME_H_
