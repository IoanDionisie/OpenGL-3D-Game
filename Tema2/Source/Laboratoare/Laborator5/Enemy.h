#pragma once

#include "GameObject.h"

class Enemy : public GameObject {
	
public:

	Enemy::Enemy();

	Enemy::~Enemy();

	int Enemy::getEnemyMove();

	bool swap1 = false, swap2 = false, swap3 = false;

	bool eraseEnemy = false;

	float rotateEnemyTime = 0;
	
	float lastEnemyX, lastEnemyY, lastEnemyZ;

	float scaleAnimationTime = 0;

	void Enemy::setEnemyMove(int enmV);


private:
	int enemyMove;

};
