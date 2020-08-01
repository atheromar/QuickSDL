#ifndef _PHYSENTITY_H
#define _PHYSENTITY_H

#include "Collider.h"
#include <vector>

class PhysicsManager;
struct PhysCollisionInfo;

class PhysEntity : public GameEntity {

protected:

	unsigned long mId;

	std::vector<Collider*> mColliders;

	Collider* mBroadPhase;

public:

	PhysEntity();
	virtual ~PhysEntity();

	unsigned long GetId();

	bool CheckCollision(PhysEntity* other, PhysCollisionInfo& colInfo);

	virtual void Hit(PhysEntity* other, PhysCollisionInfo& colInfo);

	virtual void Update();
	virtual void Render();

protected:

	virtual bool IgnoreCollisions();

	void AddCollider(Collider* collider, Vector2 localPos = VEC2_ZERO);
};

#endif