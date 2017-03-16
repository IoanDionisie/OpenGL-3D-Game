#include "Laborator5.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
float angle = RADIANS(45);
float bot = -0.5f, up = 0.5f, r = 0.5f, l = -0.5f;

Laborator5::Laborator5()
{
}

Laborator5::~Laborator5()
{
}

void Laborator5::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	player.setPositionX(0);
	player.setPositionY(1.4);
	player.setPositionZ(0.1);
	player.setScaleX(0.006f);
	player.setMovingSpeed(2.0f);
	player.setRotationAngle(0);

	tower1.setPositionX(2.5);
	tower1.setPositionY(0);
	tower1.setPositionZ(2.5);
	tower1.setScaleX(0.00006f);
	tower1.setRotationAngle(270);

	tower2.setPositionX(-2.5);
	tower2.setPositionY(0);
	tower2.setPositionZ(2.5);
	tower2.setScaleX(0.00006f);
	tower2.setRotationAngle(180);

	tower3.setPositionX(2.5);
	tower3.setPositionY(0);
	tower3.setPositionZ(-2.5);
	tower3.setScaleX(0.00006f);
	tower3.setRotationAngle(180);

	towers.push_back(tower1);
	towers.push_back(tower2);
	towers.push_back(tower3);


	timeBehind = 0;
	lastTime = 0;
	targetTimeStep = 5;

	gunBulletLastTime = 0;
	gunBulletTargetTimeStep = 0.8;

	rifleBulletLastTime = 0;
	rifleBulletTargetTimeStep = 0.4;

	grenadeBulletLastTime = 0;
	grenadeBulletTargetTimeStep = 2;

	selectWeapon = 1;

	{
		Mesh* mesh = new Mesh("base_man");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "base_man.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("tower");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "tower.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bullet");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "bullet.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("enemy");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "enemy.obj");
		meshes[mesh->GetMeshID()] = mesh;

	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator5::Update(float deltaTimeSeconds)
{

	// Render the player
	if (player.getNumberOfLives() > 0) {
		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix *= Transform3D::Translate(player.getPositionX(), player.getPositionY(), player.getPositionZ());
		modelMatrix *= Transform3D::Scale(player.getScaleX(), player.getScaleX(), player.getScaleX());
		modelMatrix *= Transform3D::RotateOY(RADIANS(180) + player.getRotationAngle());

		player.lastX = player.getPositionX();
		player.lastY = player.getPositionY();
		player.lastZ = player.getPositionZ();

		RenderMesh(meshes["base_man"], shaders["VertexNormal"], modelMatrix);
	
	} else if (rotatePlayerTime < 1.5) {	
		
		rotatePlayerTime += deltaTimeSeconds;
		glm::mat4 modelMatrix = glm::mat4(1);
	
		modelMatrix *= Transform3D::Translate(player.lastX, player.lastY, player.lastZ);
		modelMatrix *= Transform3D::Scale(player.getScaleX(), player.getScaleX(), player.getScaleX());
		modelMatrix *= Transform3D::RotateOY(RADIANS(180) + player.getRotationAngle());
		modelMatrix *= Transform3D::RotateOX(-rotatePlayerTime);
		
		RenderMesh(meshes["base_man"], shaders["VertexNormal"], modelMatrix);
	}
	
	if (rotatePlayerTime >= 1.5)
		gameOver = true;

	// Render the towers
	{
	int i;
		for (i = 0; i < towers.size(); ++i) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(towers[i].getPositionX(), player.getPositionY() - 1.4, towers[i].getPositionZ());
			modelMatrix *= Transform3D::Scale(towers[i].getScaleX(), towers[i].getScaleX(), towers[i].getScaleX());
			modelMatrix *= Transform3D::RotateOY(towers[i].getRotationAngle());
			RenderMesh(meshes["tower"], shaders["VertexNormal"], modelMatrix);
		}
		
	}

	timeBehind += Engine::GetElapsedTime() - lastTime;
	lastTime = Engine::GetElapsedTime();
	
	// Set up the enemies to be spawned once at a step time
	if (timeBehind >= targetTimeStep) {

		
		enemy.setPositionX(0 + 6.6);
		enemy.setScaleX(0.00013f);
		enemy.setScaleY(0.00013f);
		enemy.setScaleZ(0.00013f);
		enemy.setPositionY(player.getPositionY() - 1.4);
		enemy.setPositionZ(0 + 6.6);
		enemy.setRotationAngle(0);

		enemies.push_back(enemy);
		timeBehind -= targetTimeStep;


	}

	// Render the tower bullets
	for (int i = 0; i < towerBullets1.size(); ++i) {

		if (towerBullets1[i].getPositionY() >= 0 && towerBullets1.empty() != true) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(towerBullets1[i].getPositionX(), towerBullets1[i].getPositionY(), towerBullets1[i].getPositionZ());
			modelMatrix *= Transform3D::Scale(towerBullets1[i].getScaleX(), towerBullets1[i].getScaleX(), towerBullets1[i].getScaleX());
			modelMatrix *= Transform3D::RotateOY(RADIANS(towerBullets1[i].getRotationAngle()));

			gunBullets[i].setPositionX(-sin(towerBullets1[i].getRotationAngle()) *  towerBullets1[i].getMovingSpeed() * deltaTimeSeconds + towerBullets1[i].getPositionX());
			gunBullets[i].setPositionZ(-cos(towerBullets1[i].getRotationAngle()) *  towerBullets1[i].getMovingSpeed() * deltaTimeSeconds + towerBullets1[i].getPositionZ());
			towerBullets1[i].setPositionY(towerBullets1[i].getPositionY() - towerBullets1[i].shootingTime * 0.2);

			RenderMesh(meshes["bullet"], shaders["VertexNormal"], modelMatrix);
		} 
	}
	
	// Render the enemies
	for (int i = 0; i < enemies.size(); ++i) {

		if (enemies[i].eraseEnemy == false) {

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(enemies[i].getPositionX(), enemies[i].getPositionY(), enemies[i].getPositionZ());
			modelMatrix *= Transform3D::Scale(enemies[i].getScaleX(), enemies[i].getScaleX(), enemies[i].getScaleX());
			modelMatrix *= Transform3D::RotateOY(RADIANS(180) + RADIANS(enemies[i].getRotationAngle()));

		
			if (enemies[i].getPositionZ() > -5 && enemies[i].swap1 == false && enemies[i].swap2 == false && enemies[i].swap3 == false) {
				enemies[i].setPositionZ(enemies[i].getPositionZ() - 3 * deltaTimeSeconds);
			}

			if (enemies[i].getPositionZ() < -5 && enemies[i].swap1 == false && enemies[i].swap2 == false && enemies[i].swap3 == false) {
				enemies[i].swap1 = true;
			}

			if (enemies[i].swap1 == true && enemies[i].swap3 == false && enemies[i].swap2 == false) {
				enemies[i].setRotationAngle(135);
				enemies[i].setPositionX(enemies[i].getPositionX() - 3 * deltaTimeSeconds);
				enemies[i].setPositionZ(enemies[i].getPositionZ() + 3 * deltaTimeSeconds);
			}

			if (enemies[i].getPositionZ() >= 8 && enemies[i].swap1 == true && enemies[i].swap2 == false && enemies[i].swap3 == false) {
				enemies[i].swap1 = false;
				enemies[i].swap2 = true;
			}

			if (enemies[i].swap2 == true && enemies[i].swap3 == false && enemies[i].swap1 == false) {
				enemies[i].setRotationAngle(360);
				enemies[i].setPositionZ(enemies[i].getPositionZ() - 3 * deltaTimeSeconds);
			}

			if (enemies[i].getPositionZ() <= -7 && enemies[i].swap1 == false && enemies[i].swap2 == true && enemies[i].swap3 == false) {
				enemies[i].swap3 = true;
				enemies[i].swap2 = false;
				enemies[i].swap1 = false;
				player.setNumberOfLives(player.getNumberOfLives() - 1);
			}

			enemies[i].lastEnemyX = enemies[i].getPositionX();
			enemies[i].lastEnemyY = enemies[i].getPositionY();
			enemies[i].lastEnemyZ = enemies[i].getPositionZ();

			if (enemies[i].swap3 == true) {
				enemies.erase(enemies.begin() + i);
			} else {
				RenderMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix);
			}
		} else if (enemies[i].eraseEnemy == true && enemies[i].rotateEnemyTime < 1.5) {

			enemies[i].scaleAnimationTime += deltaTimeSeconds;
			enemies[i].rotateEnemyTime += deltaTimeSeconds;
			
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(enemies[i].lastEnemyX, enemies[i].lastEnemyY, enemies[i].lastEnemyZ);

			enemies[i].setScaleX(enemies[i].getScaleX() - deltaTimeSeconds * 0.00005);
			enemies[i].setScaleY(enemies[i].getScaleY() - deltaTimeSeconds * 0.00005);
			enemies[i].setScaleZ(enemies[i].getScaleZ() - deltaTimeSeconds * 0.00005);
			
			modelMatrix *= Transform3D::Scale(enemies[i].getScaleX(), enemies[i].getScaleY(), enemies[i].getScaleZ());

			modelMatrix *= Transform3D::RotateOY(RADIANS(180) + RADIANS(enemies[i].getRotationAngle()));
			modelMatrix *= Transform3D::RotateOX(-enemies[i].rotateEnemyTime);
			RenderMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix);

		}
		else if (enemies[i].rotateEnemyTime >= 1.5) {
			enemies.erase(enemies.begin() + i);
		}
	}


	gunBulletLastTime += deltaTimeSeconds;
	
	// Render the gun bullets
		for (int i = 0; i < gunBullets.size(); ++i) {
			if (gunBullets[i].eraseBullet == false && 
				gunBullets[i].getPositionX() <= 15 && gunBullets[i].getPositionX() >= -15 && 
				gunBullets[i].getPositionZ() >= -15 && gunBullets[i].getPositionZ() <= 15 &&
				abs(gunBullets[i].getPositionX() - gunBullets[i].startPlaceX) < 6 &&
				abs(gunBullets[i].getPositionZ() - gunBullets[i].startPlaceZ) < 6) {

				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(gunBullets[i].getPositionX(), gunBullets[i].getPositionY(), gunBullets[i].getPositionZ());
				modelMatrix *= Transform3D::Scale(gunBullets[i].getScaleX(), gunBullets[i].getScaleX(), gunBullets[i].getScaleX());
				modelMatrix *= Transform3D::RotateOY(RADIANS(gunBullets[i].getRotationAngle()));

				gunBullets[i].setPositionX(-sin(gunBullets[i].getRotationAngle()) *  gunBullets[i].getMovingSpeed() * deltaTimeSeconds + gunBullets[i].getPositionX());
				gunBullets[i].setPositionZ(-cos(gunBullets[i].getRotationAngle()) *  gunBullets[i].getMovingSpeed() * deltaTimeSeconds + gunBullets[i].getPositionZ());

			
				RenderMesh(meshes["bullet"], shaders["VertexNormal"], modelMatrix);
			} else {
				gunBullets.erase(gunBullets.begin() + i);
			}
		}
	
	rifleBulletLastTime += deltaTimeSeconds;

	// Render the rifle bullets
	for (int i = 0; i < rifleBullets.size(); ++i) {
		
		if (rifleBullets[i].eraseBullet == false && 
			rifleBullets[i].getPositionX() <= 15 && rifleBullets[i].getPositionY() >= -15 && 
			rifleBullets[i].getPositionZ() >= -15 && rifleBullets[i].getPositionZ() <= 15 &&
			abs(rifleBullets[i].getPositionX() - rifleBullets[i].startPlaceX) < 14 &&
			abs(rifleBullets[i].getPositionZ() - rifleBullets[i].startPlaceZ) < 14) {

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(rifleBullets[i].getPositionX(), rifleBullets[i].getPositionY(), rifleBullets[i].getPositionZ());
			modelMatrix *= Transform3D::Scale(rifleBullets[i].getScaleX(), rifleBullets[i].getScaleX(), rifleBullets[i].getScaleX());
			modelMatrix *= Transform3D::RotateOY(RADIANS(rifleBullets[i].getRotationAngle()));

			rifleBullets[i].setPositionX(-sin(rifleBullets[i].getRotationAngle()) * rifleBullets[i].getMovingSpeed() * deltaTimeSeconds + rifleBullets[i].getPositionX());
			rifleBullets[i].setPositionZ(-cos(rifleBullets[i].getRotationAngle()) *  rifleBullets[i].getMovingSpeed() * deltaTimeSeconds + rifleBullets[i].getPositionZ());
			
			RenderMesh(meshes["bullet"], shaders["VertexNormal"], modelMatrix);
		}
		else {
			rifleBullets.erase(rifleBullets.begin() + i);
		}
	}

	grenadeBulletLastTime += deltaTimeSeconds;
	
	// Render the grenade launcher bullets
	for (int i = 0; i < grenadeBullets.size(); ++i) {
		
		if (grenadeBullets[i].eraseBullet == false && 
			grenadeBullets[i].getPositionX() >= -30 && grenadeBullets[i].getPositionX() <= 30 &&
			grenadeBullets[i].getPositionZ() <= 30 && grenadeBullets[i].getPositionZ() > -30 &&
			abs(grenadeBullets[i].getPositionX() - grenadeBullets[i].startPlaceX) < 30 &&
			abs(grenadeBullets[i].getPositionZ() - grenadeBullets[i].startPlaceZ) < 30 &&
			grenadeBullets[i].grenadeTime != 2) {

			grenadeBullets[i].shootingTime += deltaTimeSeconds;

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(grenadeBullets[i].getPositionX(), grenadeBullets[i].getPositionY(), grenadeBullets[i].getPositionZ());
			modelMatrix *= Transform3D::Scale(grenadeBullets[i].getScaleX(), grenadeBullets[i].getScaleX(), grenadeBullets[i].getScaleX());
			modelMatrix *= Transform3D::RotateOY(RADIANS(grenadeBullets[i].getRotationAngle()));

			grenadeBullets[i].setPositionX(-sin(grenadeBullets[i].getRotationAngle()) *  grenadeBullets[i].getMovingSpeed() * deltaTimeSeconds + grenadeBullets[i].getPositionX());
			grenadeBullets[i].setPositionZ(-cos(grenadeBullets[i].getRotationAngle()) *   grenadeBullets[i].getMovingSpeed() * deltaTimeSeconds + grenadeBullets[i].getPositionZ());
			
			if (grenadeBullets[i].grenadeTime == 0) {
				grenadeBullets[i].setPositionY(grenadeBullets[i].getPositionY() + grenadeBullets[i].shootingTime * 0.2);
			}

			if (grenadeBullets[i].getPositionY() >= 2.5 && grenadeBullets[i].getToTop == false && grenadeBullets[i].grenadeTime == 0) {
				grenadeBullets[i].grenadeTime = 1;
			}

			if (grenadeBullets[i].grenadeTime == 1 && grenadeBullets[i].getToTop == false) {
				grenadeBullets[i].getToTop = true;
				grenadeBullets[i].grenadeTime = 4;
				grenadeBullets[i].topX = grenadeBullets[i].getPositionX();
				grenadeBullets[i].topZ = grenadeBullets[i].getPositionZ();
				grenadeBullets[i].setPositionY(grenadeBullets[i].getPositionY());
			}

			if (grenadeBullets[i].grenadeTime == 4 && grenadeBullets[i].getToTop == true &&
				abs(grenadeBullets[i].getPositionX() - grenadeBullets[i].topX) >=  0.2 &&
				abs(grenadeBullets[i].getPositionZ() - grenadeBullets[i].topZ) >= 0.2) {
					
				grenadeBullets[i].setPositionY(grenadeBullets[i].getPositionY() - grenadeBullets[i].shootingTime * 0.2);
					
			}
	

			if (grenadeBullets[i].getPositionY() <= 0) {
				grenadeBullets[i].getToTop == false;
				grenadeBullets[i].grenadeTime = 2;
			}

			RenderMesh(meshes["bullet"], shaders["VertexNormal"], modelMatrix);

		} else {
			grenadeBullets.erase(grenadeBullets.begin() + i);
		}
	}

	// Collisions with the gun bullets
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < gunBullets.size(); ++j) {
			if (sqrt((gunBullets[j].getPositionX() - enemies[i].getPositionX()) * (gunBullets[j].getPositionX() - enemies[i].getPositionX()) +
				(gunBullets[j].getPositionZ() - enemies[i].getPositionZ()) * (gunBullets[j].getPositionZ() - enemies[i].getPositionZ()) +
				(gunBullets[j].getPositionY() - enemies[i].getPositionY()) * (gunBullets[j].getPositionY() - enemies[i].getPositionY())) <
				0.00013 * 5000 + 0.002 * 350) {
				cout << " Collision " << endl;
				gunBullets[j].eraseBullet = true;
				
				enemies[i].setNumberOfLives(enemies[i].getNumberOfLives() - 1);

				if (enemies[i].getNumberOfLives() <= 0)
					enemies[i].eraseEnemy = true;
			}
		}
	}

	// Collisions with the rifle bullets
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < rifleBullets.size(); ++j) {
			if (sqrt((rifleBullets[j].getPositionX() - enemies[i].getPositionX()) * (rifleBullets[j].getPositionX() - enemies[i].getPositionX()) +
				(rifleBullets[j].getPositionZ() - enemies[i].getPositionZ()) * (rifleBullets[j].getPositionZ() - enemies[i].getPositionZ()) +
				(rifleBullets[j].getPositionY() - enemies[i].getPositionY()) * (rifleBullets[j].getPositionY() - enemies[i].getPositionY())) <
				0.00013 * 5000 + 0.003 * 310) {
				cout << " Collision " << endl;
				rifleBullets[j].eraseBullet = true;
				
				enemies[i].setNumberOfLives(enemies[i].getNumberOfLives() - 1);
				
				if (enemies[i].getNumberOfLives() <= 0)
					enemies[i].eraseEnemy = true;
			}
		}
	}

	// Collisions with the grenade bullets
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < grenadeBullets.size(); ++j) {
			if (sqrt((grenadeBullets[j].getPositionX() - enemies[i].getPositionX()) * (grenadeBullets[j].getPositionX() - enemies[i].getPositionX()) +
				(grenadeBullets[j].getPositionZ() - enemies[i].getPositionZ()) * (grenadeBullets[j].getPositionZ() - enemies[i].getPositionZ()) +
				(grenadeBullets[j].getPositionY() - enemies[i].getPositionY()) * (grenadeBullets[j].getPositionY() - enemies[i].getPositionY())) <
				0.00013 * 5000 + 0.004 * 310) {
				cout << " Collision " << endl;
				grenadeBullets[j].eraseBullet = true;
				
				enemies[i].setNumberOfLives(enemies[i].getNumberOfLives() - 4);
				
				if (enemies[i].getNumberOfLives() <= 0)
					enemies[i].eraseEnemy = true;
			}
		}
	}



}

void Laborator5::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Laborator5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_H)) {
		angle += deltaTime;
		projectionMatrix = glm::perspective(angle, window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_J)) {
		angle -= deltaTime;
		projectionMatrix = glm::perspective(angle, window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_K)) {
		bot -= deltaTime * 10;
		up += deltaTime * 10;
		projectionMatrix = glm::ortho(l, r, bot, up, 0.01f, 20.0f);
	}

	if (window->KeyHold(GLFW_KEY_L)) {
		l -= deltaTime * 10;
		r += deltaTime * 10;
		projectionMatrix = glm::ortho(l, r, bot, up, 0.01f, 20.0f);
	}

	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			player.setPositionZ(-deltaTime  * player.getMovingSpeed() * cos(player.getRotationAngle()) + player.getPositionZ());
			player.setPositionX(- deltaTime * player.getMovingSpeed() * sin(player.getRotationAngle()) + player.getPositionX());
			camera->MoveForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			player.setPositionX(-deltaTime  * player.getMovingSpeed() * cos(player.getRotationAngle()) + player.getPositionX());
			player.setPositionZ(deltaTime  * player.getMovingSpeed() * sin(player.getRotationAngle()) + player.getPositionZ());
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			player.setPositionZ(deltaTime  * player.getMovingSpeed() * cos(player.getRotationAngle()) + player.getPositionZ());
			player.setPositionX(deltaTime  * player.getMovingSpeed()  * sin(player.getRotationAngle()) + player.getPositionX());
			camera->MoveForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			player.setPositionX(deltaTime  * player.getMovingSpeed() * cos(player.getRotationAngle()) + player.getPositionX());
			player.setPositionZ(-deltaTime  * player.getMovingSpeed() * sin(player.getRotationAngle()) + player.getPositionZ());
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}
	}
}

void Laborator5::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_O)
	{
		projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 10.0f, -25.0f);
	}

	if (key == GLFW_KEY_P)
	{
		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (key == GLFW_KEY_1) {
		selectWeapon = 1;
	}

	if (key == GLFW_KEY_2) {
		selectWeapon = 2;
	}

	if (key == GLFW_KEY_3) {
		selectWeapon = 3;
	}
}

void Laborator5::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT && gameOver == false))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
			player.setRotationAngle(player.getRotationAngle() - sensivityOX * deltaX);
		}
	}
}

void Laborator5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
	// When mouse left button is clicked, a projectile is launched by the player, only if he is still alive
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && gameOver == false) {

		if (selectWeapon == 1 && gunBulletLastTime >= gunBulletTargetTimeStep) {
			Bullet bullet1;
			bullet1.setPositionX(player.getPositionX());
			bullet1.setPositionY(1.2);
			bullet1.setPositionZ(player.getPositionZ());
			bullet1.setScaleX(0.002);
			bullet1.setRotationAngle(player.getRotationAngle());

			bullet1.startPlaceX = player.getPositionX();
			bullet1.startPlaceZ = player.getPositionZ();

			gunBullets.push_back(bullet1);

			gunBulletLastTime = 0;
		
		} else if (selectWeapon == 2 && rifleBulletLastTime >= rifleBulletTargetTimeStep) {
			Bullet bullet2;
			bullet2.setPositionX(player.getPositionX());
			bullet2.setPositionY(1.2);
			bullet2.setPositionZ(player.getPositionZ());
			bullet2.setScaleX(0.003);
			bullet2.setRotationAngle(player.getRotationAngle());

			bullet2.startPlaceX = player.getPositionX();
			bullet2.startPlaceZ = player.getPositionZ();

			rifleBullets.push_back(bullet2);

			rifleBulletLastTime = 0;
		
		} else if (selectWeapon == 3 && grenadeBulletLastTime >= grenadeBulletTargetTimeStep) {
			Bullet bullet3;
			bullet3.setPositionX(player.getPositionX());
			bullet3.setPositionY(0.4);
			bullet3.setPositionZ(player.getPositionZ());
			bullet3.setScaleX(0.015);
			bullet3.setRotationAngle(player.getRotationAngle());
			
			bullet3.startPlaceX = player.getPositionX();
			bullet3.startPlaceZ = player.getPositionZ();
			bullet3.shootingTime = 0;

			grenadeBullets.push_back(bullet3);

			grenadeBulletLastTime = 0;
		}

	}
}

void Laborator5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}

void Laborator5::OnWindowResize(int width, int height)
{
}
