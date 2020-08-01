#ifndef _PHYSICSMANAGER_H
#define _PHYSICSMANAGER_H

#include "PhysEntity.h"
#include <bitset>

class PhysicsManager {

public:

	enum class CollisionLayer {

		Friendly = 0,
		FriendlyProjectiles,
		Hostile,
		HostileProjectiles,
		//------------------------------------------------------
		MaxLayers
	};

	enum class CollisionFlags {

		None				= 0x00,
		Friendly			= 0x01,
		FriendlyProjectiles = 0x02,
		Hostile				= 0x04,
		HostileProjectiles	= 0x08
	};

private:

	static PhysicsManager* sInstance;

	std::vector<PhysEntity*> mCollisionLayers[static_cast<unsigned int>(CollisionLayer::MaxLayers)];
	std::bitset<static_cast<unsigned int>(CollisionLayer::MaxLayers)> mLayerMasks[static_cast<unsigned int>(CollisionLayer::MaxLayers)];

	unsigned long mLastId;

public:

	static PhysicsManager* Instance();
	static void Release();

	void SetLayerCollisionMask(CollisionLayer layer, CollisionFlags flags);

	unsigned long RegisterEntity(PhysEntity* entity, CollisionLayer layer);
	void UnregisterEntity(unsigned long id);
	void Update();

private:

	PhysicsManager();
	~PhysicsManager();
};

inline PhysicsManager::CollisionFlags operator|(PhysicsManager::CollisionFlags a, PhysicsManager::CollisionFlags b) {

	return static_cast<PhysicsManager::CollisionFlags>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline PhysicsManager::CollisionFlags operator&(PhysicsManager::CollisionFlags a, PhysicsManager::CollisionFlags b) {

	return static_cast<PhysicsManager::CollisionFlags>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

#endif // _PHYSICSMANAGER_H