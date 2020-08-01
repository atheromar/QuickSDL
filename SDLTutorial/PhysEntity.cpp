#include "PhysEntity.h"
#include "PhysicsManager.h"
#include "PhysicsHelper.h"

PhysEntity::PhysEntity() {

	mBroadPhase = nullptr;
	mId = 0;
}

PhysEntity::~PhysEntity() {

	for(unsigned int i = 0; i < mColliders.size(); i++) {

		delete mColliders[i];
		mColliders[i] = nullptr;
	}

	mColliders.clear();

	if(mBroadPhase) {

		delete mBroadPhase;
		mBroadPhase = nullptr;
	}

	if(mId != 0) {

		PhysicsManager::Instance()->UnregisterEntity(mId);
	}
}

unsigned long PhysEntity::GetId() {

	return mId;
}

bool PhysEntity::CheckCollision(PhysEntity* other, PhysCollisionInfo& colInfo) {

	if(IgnoreCollisions() || other->IgnoreCollisions())
		return false;
	
	bool narrowPhaseCheck = false;
	if (mBroadPhase && other->mBroadPhase)
	{
		narrowPhaseCheck = ColliderColliderCheck(mBroadPhase, other->mBroadPhase, colInfo);
	}
	else
	{
		narrowPhaseCheck = true;
	}

	if (narrowPhaseCheck)
	{
		for (unsigned int i = 0; i < mColliders.size(); i++)
		{
			for (unsigned int j = 0; j < other->mColliders.size(); j++)
			{
				if (ColliderColliderCheck(mColliders[i], other->mColliders[j], colInfo))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void PhysEntity::Hit(PhysEntity* other, PhysCollisionInfo& colInfo) {

}

bool PhysEntity::IgnoreCollisions() {

	return false;
}

void PhysEntity::AddCollider(Collider* collider, Vector2 localPos) {

	int index = mColliders.size();
	mColliders.push_back(collider);
	mColliders[index]->Parent(this);
	mColliders[index]->Pos(localPos);

	if(mColliders.size() > 1 || mColliders[0]->GetType() == Collider::ColliderType::Box) {

		float furthestDist = mColliders[0]->GetFurthestPoint().Magnitude();
		float dist = 0.0f;
		for(unsigned int i = 1; i < mColliders.size(); i++) {

			dist = mColliders[i]->GetFurthestPoint().Magnitude();
			if(dist > furthestDist) {

				furthestDist = dist;
			}
		}

		delete mBroadPhase;
		mBroadPhase = new CircleCollider(furthestDist, true);
		mBroadPhase->Parent(this);
		mBroadPhase->Pos(VEC2_ZERO);
	}
}

void PhysEntity::Update() {

	for(unsigned int i = 0; i < mColliders.size(); i++) {

		mColliders[i]->Update();
	}
}

void PhysEntity::Render() {

	for(unsigned int i = 0; i < mColliders.size(); i++) {

		mColliders[i]->Render();
	}

	if(mBroadPhase)
		mBroadPhase->Render();
}