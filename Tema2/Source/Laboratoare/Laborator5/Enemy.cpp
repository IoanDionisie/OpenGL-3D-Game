#include <iostream>
#include <stdlib.h>     
#include <time.h>  
#include <string>
#include "Enemy.h"


Enemy::Enemy() {
	setNumberOfLives(4);
	setMovingSpeed(rand() % 200 + 75);
	enemyMove = 1;
}

Enemy::~Enemy() {

}

int Enemy::getEnemyMove() {
	return enemyMove;
}
void Enemy::setEnemyMove(int enmV) {
	enemyMove = enmV;
}


