#pragma once
#include <string>
#include <Core/Engine.h>
#include "Component/SimpleScene.h"

class GameObject {

public:

	GameObject();
	~GameObject();


	int enemyMove;
	
	float getTranslateX();
	float getTranslateY();
	float getTranslateZ();
	float getScaleX();
	float getScaleY();
	float getScaleZ();
	float getPositionX();
	float getPositionY();
	float getPositionZ();

	float getRotationAngle();

	int getNumberOfLives();
	float getMovingSpeed();

	void setTranslateX(float translateX);
	void setTranslateY(float translateY);
	void setTranslateZ(float translateZ);
	void setScaleX(float scaleX);
	void setScaleY(float translateY);
	void setScaleZ(float translateZ);
	void setPositionX(float positionX);
	void setPositionY(float positionY);
	void setPositionZ(float positionZ);
	void setRotationAngle(float rotationAngle);
	void setNumberOfLives(int numberOfLives);
	void setMovingSpeed(float mvSpeed);

protected:

	float translateX, translateY, translateZ;
	float scaleX, scaleY, scaleZ;
	float positionX, positionY, positionZ;
	float rotationAngle;
	float movingSpeed;

	int numberOfLives;

};

#pragma once
