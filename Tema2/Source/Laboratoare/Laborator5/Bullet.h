#pragma once

#include <iostream>
#include "GameObject.h"


class Bullet : public GameObject {

public:
	
	bool eraseBullet = false;
	
	float startPlaceX;
	float startPlaceZ;
	float grenadeTime = 0;
	float shootingTime;
	float topX, topZ;
	bool getToTop = false;

	Bullet();
	
	~Bullet();

};
