//-------------------------------------------------------------------------------------------------------------------//
// GameEntity.cpp                                                                                                    //
// The base class for all game entities, providing position, rotation, and scaling functionality to all entities.    //
//                                                                                                                   //
// Sets up a system to parent GameEntity objects to one another                                                      //
// making the child's position, rotation, and scale relative to the parent's instead of the world origin             //
//                                                                                                                   //
// By: Ather Omar                                                                                                    //
//-------------------------------------------------------------------------------------------------------------------//
#include "GameEntity.h"
//-------------------------------------------------------------
// QuickSDL
//-------------------------------------------------------------
namespace QuickSDL {

	GameEntity::GameEntity(Vector2 pos) {

		mPos = pos;
		mRotation = 0.0f;
		mActive = true;
		mParent = NULL;
		mScale = VEC2_ONE;
	}

	GameEntity::~GameEntity() {

		mParent = NULL;
	}

	void GameEntity::Pos(Vector2 pos) {

		mPos = pos;
	}

	Vector2 GameEntity::Pos(SPACE space) {

		if(space == local || mParent == NULL)
			return mPos;

		GameEntity* parent = mParent;
		Vector2 finalPos = mPos, parentScale = VEC2_ZERO;

		do 
		{
			parentScale = parent->Scale(local);
			finalPos = RotateVector(Vector2(finalPos.x * parentScale.x, finalPos.y * parentScale.y), parent->Rotation(local));
			finalPos += parent->Pos(local);

			parent = parent->Parent();

		} while (parent);
		
		//The final position also depends on the parent's scale (if the parent is scaled up, the object should be further away from the parent)
		return finalPos;
	}

	void GameEntity::Rotation(float r) {

		mRotation = r;

		//Wraps the angle between 0 and 360 degrees, addition and subtraction is done to avoid snapping
		//Updated to deal with degrees higher than 360 and -360
		if(mRotation > 360.0f) {

			int mul = (int)(mRotation / 360);
			mRotation -= 360.0f * mul;

		} else if(mRotation < 0.0f) {

			int mul = (int)((mRotation / 360) - 1);
			mRotation -= 360.0f * mul;
		}
	}

	float GameEntity::Rotation(SPACE space) {

		if(space == local || mParent == NULL)
			return mRotation;

		return mParent->Rotation(world) + mRotation;
	}

	void GameEntity::Scale(Vector2 scale) {

		mScale = scale;
	}

	Vector2 GameEntity::Scale(SPACE space) {

		if(space == local || mParent == NULL)
			return mScale;

		Vector2 scale = mParent->Scale(world);
		scale.x *= mScale.x;
		scale.y *= mScale.y;

		return scale;
	}

	void GameEntity::Active(bool active) {

		mActive = active;
	}

	bool GameEntity::Active() {

		return mActive;
	}

	void GameEntity::Parent(GameEntity* parent) {

		//If the parent is removed, The Position/Rotation/Scale are the GameEntity's world values, to keep the object looking the same after the removal of the parent;
		if(parent == NULL) {

			mPos = Pos(world);
			mScale = Scale(world);
			mRotation = Rotation(world);

		} else {

			//If the object already has a parent, remove the current parent to get it ready to be the child for the new parent
			if(mParent != NULL)
				Parent(NULL);

			Vector2 parentScale = parent->Scale(world);

			//Setting the local position to be relative to the new parent (while maintaining the same world position as before)
			mPos = RotateVector(Pos(world) - parent->Pos(world), -parent->Rotation(world));
			mPos.x /= parentScale.x;
			mPos.y /= parentScale.y;

			//Setting the local rotation to be relative to the new parent (while maintaining the same world rotation as before)
			mRotation = mRotation - parent->Rotation(world);

			//Setting the new scale to be relative to the new parent (while maintaining the same world scale as before)
			mScale = Vector2(mScale.x / parentScale.x, mScale.y / parentScale.y);
		}

		mParent = parent;
	}

	GameEntity* GameEntity::Parent() {

		return mParent;
	}

	void GameEntity::Translate(Vector2 vec, SPACE space) {

		if(space == world) {
			mPos += vec;
		} else {

			mPos += RotateVector(vec, Rotation());
		}
	}

	void GameEntity::Rotate(float amount) {

		Rotation(mRotation + amount);
	}

	void GameEntity::Update() {


	}

	void GameEntity::Render() {


	}
}