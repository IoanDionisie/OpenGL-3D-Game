#include <iostream>
#include "GameObject.h"




GameObject::GameObject() {
}

GameObject::~GameObject() {

}

float GameObject::getTranslateX() {
	return translateX;
}

float GameObject::getTranslateY() {
	return translateY;
}


float GameObject::getTranslateZ() {
	return translateZ;
}

float GameObject::getScaleX() {
	return scaleX;
}

float GameObject::getScaleY() {
	return scaleY;
}

float GameObject::getScaleZ() {
	return scaleZ;
}

float GameObject::getPositionX() {
	return positionX;
}

float GameObject::getPositionY() {
	return positionY;
}


float GameObject::getPositionZ() {
	return positionZ;
}

int GameObject::getNumberOfLives() {
	return numberOfLives;
}

float GameObject::getRotationAngle() {
	return rotationAngle;
}

float GameObject::getMovingSpeed() {
	return movingSpeed;
}

void GameObject::setTranslateX(float trX) {
	translateX = trX;
}

void GameObject::setTranslateY(float trY) {
	translateY = trY;
}

void GameObject::setTranslateZ(float trZ) {
	translateZ = trZ;
}

void GameObject::setScaleX(float scX) {
	scaleX = scX;
}

void GameObject::setScaleY(float scY) {
	scaleY = scY;
}

void GameObject::setScaleZ(float scZ) {
	scaleZ = scZ;
}

void GameObject::setPositionX(float posX) {
	positionX = posX;
}

void GameObject::setPositionY(float posY) {
	positionY = posY;
}

void GameObject::setPositionZ(float posZ) {
	positionZ = posZ;
}

void GameObject::setRotationAngle(float rot) {
	rotationAngle = rot;
}

void GameObject::setNumberOfLives(int nrLives) {
	numberOfLives = nrLives;
}

void GameObject::setMovingSpeed(float mvSpeed) {
	movingSpeed = mvSpeed;
}
