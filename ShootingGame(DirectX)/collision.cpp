#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "enemy.h"
#include "player.h"
#include "boss.h"
#include "item.h"
#include "explosion.h"
#include "scoreboard.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "main.h"
#include "mydirect3d.h"

bool circleCollsion(const Circle* pA, const Circle* pB) {
	return D3DXVec2LengthSq(&(pB->position - pA->position)) < (pA->radius + pB->radius)*(pA->radius + pB->radius);
}

void checkCollisionAll() {
	float pPositionX, pPositionY;
	float ePositionX, ePositionY;
	float pBulletPositionX, pBulletPositionY;
	float eBulletPositionX, eBulletPositionY;

	//Check the collision between player's bullets and the enemies
	for (int i = 0; i < PSHOT_NUM; i++) {
		if (getPlayerShotPosition(i, &pBulletPositionX, &pBulletPositionY)) {
			for (int j = 0; j < ENEMY_NUM; j++) {
				if (g_FrameCount > getEnemyInTime(j) && !getEnemyDeadFlag(j)) {
					getEnemyPosition(j, &ePositionX, &ePositionY);
					Circle pBullet, enemyObj;
					pBullet.position.x = pBulletPositionX;
					pBullet.position.y = pBulletPositionY;
					pBullet.radius = PSHOT_COLLISIONRAD;
					enemyObj.position.x = ePositionX;
					enemyObj.position.y = ePositionY;
					enemyObj.radius = ENEMY_COLLISIONRAD;

					if (circleCollsion(&pBullet, &enemyObj)) {
						//Decrease enemy's hp
						setEnemyHp(j, -1);
						//set the bullet that hit the enemy to false
						setPlayerShotFlag(i, false);
						//If enemy's hp comes to 0
						if (getEnemyHP(j) == 0) {
							//set deadFlag to true
							setEnemyDeadFlag(j);
							//decrease enemyLeft
							setEnemyLeft(1);
							//play enemy destroy sound effect
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
							//get points
							setScoreData(CURRENT_SCORE, 100);
							//item appears
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									setItemFlag(k, ePositionX - 2.5f, ePositionY, getEnemyItemType(j));
									setItemFlag(k, ePositionX + 2.5f, ePositionY, getEnemyItemType(j));
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	//player's collision between enemy's bulltes
	if (!getPlayerDamageFlag() && !getPlayerInvincibleFlag() && getPlayerLife() > 0) {
		getPlayerPosition(&pPositionX, &pPositionY);

		//Check the collision between enemies' bullets and the player
		for (int i = 0; i < ENEMY_NUM; i++) {
			for (int j = 0; j < ENEMY_SNUM; j++) {
				//Check if the bullet is used, if it is used then get its position
				if (getEnemyShotPosition(i, j, &eBulletPositionX, &eBulletPositionY)) {
					Circle eBullet, playerObj;
					eBullet.position.x = eBulletPositionX;
					eBullet.position.y = eBulletPositionY;
					switch (getEnemyShotType(i)) {
					case 0:
						eBullet.radius = ESHOT0_COLLISIONRAD;
						break;
					case 1:
						eBullet.radius = ESHOT1_COLLISIONRAD;
						break;
					case 2:
						eBullet.radius = ESHOT2_COLLISIONRAD;
						break;
					default: break;
					}
					playerObj.position.x = pPositionX;
					playerObj.position.y = pPositionY;
					playerObj.radius = PLAYER_COLLISIONRAD;

					if (circleCollsion(&eBullet, &playerObj)) {
						//if the player was hit, set player's damage flag to true
						setPlayerDamageFlag();
						//player lose some of the power
						setPlayerPower(-2);
						//set the bullet that hit the player to false
						setEnemyShotFlag(i, j, false);
						//play player destroy sound effect
						PlaySound(SOUND_LABEL_SE_DEATH);
						break;
					}
				}
			}
		}
	}

	//Player's collision between the items
	if (!getPlayerDamageFlag() && getPlayerLife() > 0) {
		getPlayerPosition(&pPositionX, &pPositionY);

		//Check the collision between player and the items
		float itemX, itemY;
		for (int i = 0; i < ITEM_NUM; i++) {
			if (getItemFlag(i)) {
				getItemPosition(i, &itemX, &itemY);
				Circle playerObj, itemObj;
				playerObj.position.x = pPositionX;
				playerObj.position.y = pPositionY;
				playerObj.radius = PLAYER_COLLISIONRAD + 10;
				itemObj.position.x = itemX;
				itemObj.position.y = itemY;
				itemObj.radius = ITEM_COLLISIONRAD;

				if (circleCollsion(&playerObj, &itemObj)) {
					switch (getItemType(i)) {
					case 0:			//Get power
						setPlayerPower(1);
						break;
					case 1:			//Get point
						setScoreData(CURRENT_SCORE, 256);
						break;
					case 2:			//Get 1up
						setPlayerLife();
						break;
					default: break;
					}

					PlaySound(SOUND_LABEL_SE_GETITEM);
					DestroyItem(i);
				}
			}
		}
	}

	//get and set player's life and power
	setScoreData(LIFE_SCORE, getPlayerLife());
	setScoreData(POWER_SCORE, getPlayerPower());
}



//Check the collsions between the player and the boss
void checkBossCollision() {
	float pPositionX, pPositionY;
	float bPositionX, bPositionY;
	float pBulletPositionX, pBulletPositionY;
	float bBulletPositionX, bBulletPositionY;
	float itemPositionX, itemPositionY;
	//Boss‚Ì’e‚ÌŽí—Þ
	int bossShotType;

	int itemNum = 0;

	//boss's collision between player's bullets
	for (int i = 0; i < PSHOT_NUM; i++) {
		if (getPlayerShotPosition(i, &pBulletPositionX, &pBulletPositionY)) {
			if (getBossFlag() && getBossAppearFlag() && !getBossInvincibleFlag()) {
				getBossPosition(&bPositionX, &bPositionY);
				Circle pBullet, bossObj;
				pBullet.position.x = pBulletPositionX;
				pBullet.position.y = pBulletPositionY;
				pBullet.radius = PSHOT_COLLISIONRAD;
				bossObj.position.x = bPositionX;
				bossObj.position.y = bPositionY;
				bossObj.radius = BOSS_COLLISIONRAD;

				if (circleCollsion(&pBullet, &bossObj)) {
					//if boss was hit, decrease its hp
					int bossHp = setBossHp(1);
					//set the bullet that hit the boss to false
					setPlayerShotFlag(i, false);
					//“¾“_‚ð‰Á‚¦‚é
					setScoreData(CURRENT_SCORE, 512);

					if (getGameDifficulty() != DIFFICULTY_LUNATIC) {
						//Change to move pattern 2
						if (BOSS_HP * 2 / 3.f >= bossHp && getPrevBossHp() > BOSS_HP * 2 / 3.f) {
							setScoreData(CURRENT_SCORE, 1024);
							//Spawn items
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									itemPositionX = bPositionX + ((rand() % 100) - 51);
									itemPositionY = bPositionY + ((rand() % 100) - 51);
									if (itemNum == 0 || itemNum == 1)
										setItemFlag(k, itemPositionX, itemPositionY, 2);
									else
										setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

									itemNum++;
									if (itemNum == 10) {
										itemNum = 0;
										break;
									}
								}
							}
							setBossDamageSetting();
						}
						//Change to move pattern 3
						else if (BOSS_HP / 3.f >= bossHp && getPrevBossHp() > BOSS_HP / 3.f) {
							setScoreData(CURRENT_SCORE, 1024);
							//Spawn items
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									itemPositionX = bPositionX + ((rand() % 100) - 51);
									itemPositionY = bPositionY + ((rand() % 100) - 51);
									if (itemNum == 0 || itemNum == 1)
										setItemFlag(k, itemPositionX, itemPositionY, 2);
									else
										setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

									itemNum++;
									if (itemNum == 10) {
										itemNum = 0;
										break;
									}
								}
							}
							setBossDamageSetting();
						}
					}
					else {
						//Change to move pattern 2
						if (BOSS_HP * 3 / 4.f >= bossHp && getPrevBossHp() > BOSS_HP * 3 / 4.f) {
							setScoreData(CURRENT_SCORE, 1024);
							//Spawn items
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									itemPositionX = bPositionX + ((rand() % 100) - 51);
									itemPositionY = bPositionY + ((rand() % 100) - 51);
									if (itemNum == 0 || itemNum == 1)
										setItemFlag(k, itemPositionX, itemPositionY, 2);
									else
										setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

									itemNum++;
									if (itemNum == 15) {
										itemNum = 0;
										break;
									}
								}
							}
							setBossDamageSetting();
						}
						//Change to move pattern 3
						else if (BOSS_HP * 2 / 4.f >= bossHp && getPrevBossHp() > BOSS_HP * 2 / 4.f) {
							setScoreData(CURRENT_SCORE, 1024);
							//Spawn items
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									itemPositionX = bPositionX + ((rand() % 100) - 51);
									itemPositionY = bPositionY + ((rand() % 100) - 51);
									if (itemNum == 0 || itemNum == 1)
										setItemFlag(k, itemPositionX, itemPositionY, 2);
									else
										setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

									itemNum++;
									if (itemNum == 15) {
										itemNum = 0;
										break;
									}
								}
							}
							setBossDamageSetting();
						}
						//Change to move pattern 4
						else if (BOSS_HP / 4.f >= bossHp && getPrevBossHp() > BOSS_HP / 4.f) {
							setScoreData(CURRENT_SCORE, 1024);
							//Spawn items
							for (int k = 0; k < ITEM_NUM; k++) {
								if (!getItemFlag(k)) {
									itemPositionX = bPositionX + ((rand() % 100) - 51);
									itemPositionY = bPositionY + ((rand() % 100) - 51);
									if (itemNum == 0 || itemNum == 1)
										setItemFlag(k, itemPositionX, itemPositionY, 2);
									else
										setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

									itemNum++;
									if (itemNum == 15) {
										itemNum = 0;
										break;
									}
								}
							}
							setBossDamageSetting();
						}
					}


					//If the boss died
					if (bossHp <= 0) {
						//Set boss's alive flag to false
						setBossFlag(false);
						//Play boss explosion effect
						CreateExplosion(bossObj.position.x, bossObj.position.y);
						//Get Point!!!
						setScoreData(CURRENT_SCORE, 100000);
						//Spawn items
						for (int k = 0; k < ITEM_NUM; k++) {
							if (!getItemFlag(k)) {
								itemPositionX = bPositionX + ((rand() % 100) - 51);
								itemPositionY = bPositionY + ((rand() % 100) - 51);
								setItemFlag(k, itemPositionX, itemPositionY, rand() % 2);

								itemNum++;
								if (itemNum == 10) {
									itemNum = 0;
									break;
								}
							}
						}
						//Play explosion SE
						PlaySound(SOUND_LABEL_SE_BOSSDEATH);

						//Unlock lunatic mode if hard mode was cleared
						if (getGameDifficulty() == DIFFICULTY_HARD)
							setLunaLock(false);
					}
				}
			}
		}
	}

	//player's collision between boss's bullets
	if (!getPlayerDamageFlag() && !getPlayerInvincibleFlag() && getPlayerLife() > 0) {
		getPlayerPosition(&pPositionX, &pPositionY);

		for (int i = 0; i < BOSS_SHOTNUM; i++) {
			if (getBossShotPosition(i, &bBulletPositionX, &bBulletPositionY, &bossShotType)) {
				Circle bBullet, playerObj;
				bBullet.position.x = bBulletPositionX;
				bBullet.position.y = bBulletPositionY;
				switch (bossShotType)
				{
				case 0:
					bBullet.radius = 15;
					break;
				case 1:
					bBullet.radius = 15;
					break;
				case 2:
					bBullet.radius = 18;
					break;
				case 3:
					bBullet.radius = 3;
					break;
				default: break;
				}
				playerObj.position.x = pPositionX;
				playerObj.position.y = pPositionY;
				playerObj.radius = PLAYER_COLLISIONRAD;

				if (circleCollsion(&bBullet, &playerObj)) {
					//if the player was hit, set player's damage flag to true
					setPlayerDamageFlag();
					//player lose some of the power
					setPlayerPower(-2);
					//set the bullet that hit the player to false
					setBossShotFlag(i, false);
					//play player destroy sound effect
					PlaySound(SOUND_LABEL_SE_DEATH);
					break;
				}

			}
		}
	}

	//Player's collision between the items
	if (!getPlayerDamageFlag() && getPlayerLife() > 0) {
		getPlayerPosition(&pPositionX, &pPositionY);
		
		//Check the collision between player and the items
		float itemX, itemY;
		for (int i = 0; i < ITEM_NUM; i++) {
			if (getItemFlag(i)) {
				getItemPosition(i, &itemX, &itemY);
				Circle playerObj, itemObj;
				playerObj.position.x = pPositionX;
				playerObj.position.y = pPositionY;
				playerObj.radius = PLAYER_COLLISIONRAD + 10;
				itemObj.position.x = itemX;
				itemObj.position.y = itemY;
				itemObj.radius = ITEM_COLLISIONRAD;

				if (circleCollsion(&playerObj, &itemObj)) {
					switch (getItemType(i)) {
					case 0:			//Get power
						setPlayerPower(1);
						break;
					case 1:			//Get point
						setScoreData(CURRENT_SCORE, 256);
						break;
					case 2:			//Get 1up
						setPlayerLife();
						break;
					default: break;
					}

					PlaySound(SOUND_LABEL_SE_GETITEM);
					DestroyItem(i);
				}
			}
		}
	}

	//get and set player's life and power
	setScoreData(LIFE_SCORE, getPlayerLife());
	setScoreData(POWER_SCORE, getPlayerPower());
}