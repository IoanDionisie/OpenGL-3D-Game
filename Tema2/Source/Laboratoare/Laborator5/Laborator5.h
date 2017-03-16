#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Tower.h"

class Laborator5 : public SimpleScene
{
public:
	Laborator5();
	~Laborator5();

	void Init() override;

	Player player;
	 
	Tower tower1, tower2, tower3;

	std::vector<Tower> towers;
	std::vector<Enemy> enemies;
	std::vector<Bullet> towerBullets1;
	std::vector<Bullet> towerBullets2;
	std::vector<Bullet> towerBullets3;
	std::vector<Bullet> gunBullets;
	std::vector<Bullet> rifleBullets;
	std::vector<Bullet> grenadeBullets;
	Enemy enemy;

	float rotatePlayerTime;
	bool gameOver = false;

	int selectWeapon;

	float lastTime;
	float timeBehind;
	float targetTimeStep;

	float gunBulletLastTime;
	float gunBulletTargetTimeStep;

	float rifleBulletLastTime;
	float rifleBulletTargetTimeStep;

	float grenadeBulletLastTime;
	float grenadeBulletTargetTimeStep;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Laborator::Camera *camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
};
