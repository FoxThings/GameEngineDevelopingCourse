#pragma once

#include <flecs.h>
#include <Timer.h>

void RegisterEcsGameplaySystems(flecs::world& world);

struct Player
{
	int maxAmmoCount;
	float timeToRecharge;
	float timeToShoot;
	int curAmmoCount;
	float lastRecharged;
	float lastShooted;
};

struct LocalTimer
{
	GameEngine::Core::Timer timer;
};

struct Bullet
{
	float timeToDeath;
};

struct Obstacle
{
	std::function<void()> onCollision;
};
