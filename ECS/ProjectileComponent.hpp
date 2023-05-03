//Component used for projectiles

#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include "../Vector2D.hpp"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D vel) : velocity(vel)
	{}
	~ProjectileComponent()
	{};

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		if (transform->position.y < 0 || transform->position.y > 640)
		{
			entity->destroy();
		}
	}

	bool hasCollided() { return collided; }
	void setCollided() { collided = true; }

private:
	TransformComponent* transform;
	Vector2D velocity;
	bool collided = false;
};