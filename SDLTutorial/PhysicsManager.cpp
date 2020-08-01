#include "PhysicsManager.h"
#include "PhysicsHelper.h"

PhysicsManager* PhysicsManager::sInstance = nullptr;

PhysicsManager* PhysicsManager::Instance() {
	
	if(sInstance == nullptr)
		sInstance = new PhysicsManager();

	return sInstance;
}

void PhysicsManager::Release() {

	delete sInstance;
	sInstance = nullptr;
}

void PhysicsManager::SetLayerCollisionMask(CollisionLayer layer, CollisionFlags flags) {

	mLayerMasks[static_cast<unsigned int>(layer)] = std::bitset<static_cast<unsigned int>(CollisionLayer::MaxLayers)>(static_cast<unsigned int>(flags));
}

unsigned long PhysicsManager::RegisterEntity(PhysEntity* entity, CollisionLayer layer) {

	mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);
	
	mLastId++;

	return mLastId;
}

void PhysicsManager::UnregisterEntity(unsigned long id) {

	bool found = false;
	for(unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayer::MaxLayers) && !found; i++) {

		for(unsigned int j = 0; j < mCollisionLayers[i].size() && !found; j++) {

			if(mCollisionLayers[i][j]->GetId() == id) {

				mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
				found = true;
			}
		}
	}
}

PhysicsManager::PhysicsManager() {

	for(unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayer::MaxLayers); i++) {

		mLayerMasks[i] = static_cast<unsigned int>(CollisionFlags::None);
	}

	mLastId = 0;
}

PhysicsManager::~PhysicsManager() {

	for(unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayer::MaxLayers); i++) {

		mCollisionLayers[i].clear();
	}
}

void PhysicsManager::Update() {

	PhysCollisionInfo colInfo;
	for(unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayer::MaxLayers); i++) {

		for(unsigned int j = 0; j < static_cast<unsigned int>(CollisionLayer::MaxLayers); j++) {

			if(mLayerMasks[i].test(j) && i <= j) {

				for(unsigned int k = 0; k < mCollisionLayers[i].size(); k++) {

					for(unsigned int l = 0; l < mCollisionLayers[j].size(); l++) {

						if(mCollisionLayers[i][k]->Active() && mCollisionLayers[i][k]->CheckCollision(mCollisionLayers[j][l], colInfo)) {

							mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l], colInfo);
							colInfo.normal = -colInfo.normal;
							mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k], colInfo);
						}
					}
				}
			}
		}
	}
}