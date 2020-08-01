#ifndef _PHYSICSHELPER_H
#define _PHYSICSHELPER_H

#include "CircleCollider.h"
#include "BoxCollider.h"
#include "MathHelper.h"

struct PhysCollisionInfo {

	PhysCollisionInfo()
		: overlap(0.0f), normal(VEC2_ZERO)
	{}

	float overlap;
	Vector2 normal;
};

inline bool PointInPolygon(Vector2* verts, int vertCount, const Vector2& point)
{
	bool retVal = false;

	for (int i = 0, j = vertCount - 1; i < vertCount; j = i++)
	{
		if ((verts[i].y >= point.y) != (verts[j].y >= point.y))
		{
			Vector2 vec1 = (verts[i] - verts[j]).Normalized();
			Vector2 proj = verts[j] + vec1 * Dot(point - verts[j], vec1);
			if (proj.x > point.x)
			{
				retVal = !retVal;
			}
		}
	}

	return retVal;
}

inline float PointToLineDistance(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& point)
{
	Vector2 delta = lineEnd - lineStart;
	
	float param = Clamp(Dot(point - lineStart, delta) / delta.MagnitudeSqr(), 0.0f, 1.0f);
	
	Vector2 nearestpoint = lineStart + delta*param;

	return (point - nearestpoint).Magnitude();
}

inline bool CircleCircleCollision(CircleCollider* c1, CircleCollider* c2, PhysCollisionInfo& colInfo) {

	Vector2 dist = c1->Pos() - c2->Pos();
	colInfo.normal = dist.Normalized();
	colInfo.overlap = (c1->GetRadius() + c2->GetRadius()) - dist.Magnitude();
	return colInfo.overlap > 0.0f;
}

inline bool BoxCircleCollision(BoxCollider* box, CircleCollider* circle, PhysCollisionInfo& colInfo) {

	float radius = circle->GetRadius();
	Vector2 circlePos = circle->Pos(GameEntity::world);

	if (PointToLineDistance(box->GetVertexPos(0), box->GetVertexPos(1), circlePos) < radius ||
		PointToLineDistance(box->GetVertexPos(0), box->GetVertexPos(2), circlePos) < radius ||
		PointToLineDistance(box->GetVertexPos(2), box->GetVertexPos(3), circlePos) < radius ||
		PointToLineDistance(box->GetVertexPos(3), box->GetVertexPos(1), circlePos) < radius)
	{
		return true;
	}

	Vector2 quad[4];
	quad[0] = box->GetVertexPos(0);
	quad[1] = box->GetVertexPos(1);
	quad[2] = box->GetVertexPos(3);
	quad[3] = box->GetVertexPos(2);

	for (int i = 0; i < 4; i++)
	{
		if ((quad[i] - circlePos).Magnitude() < radius)
		{
			return true;
		}
	}

	if (PointInPolygon(quad, 4, circlePos))
	{
		return true;
	}

	return false;
}

inline bool BoxBoxCollision(BoxCollider* b1, BoxCollider* b2, PhysCollisionInfo& colInfo) {

	Vector2 projAxis[4];

	projAxis[0] = (b1->GetVertexPos(0) - b1->GetVertexPos(1)).Normalized();
	projAxis[1] = (b1->GetVertexPos(0) - b1->GetVertexPos(2)).Normalized();
	projAxis[2] = (b2->GetVertexPos(0) - b2->GetVertexPos(1)).Normalized();
	projAxis[3] = (b2->GetVertexPos(0) - b2->GetVertexPos(2)).Normalized();

	float b1Min = 0.0f, b1Max = 0.0f;
	float b2Min = 0.0f, b2Max = 0.0f;
	float proj1 = 0.0f, proj2 = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			proj1 = Dot(b1->GetVertexPos(j), projAxis[i]);
			proj2 = Dot(b2->GetVertexPos(j), projAxis[i]);

			if (j == 0)
			{
				b1Min = b1Max = proj1;

				b2Min = b2Max = proj2;
			}
			else
			{
				if (proj1 < b1Min)
					b1Min = proj1;
				if (proj1 > b1Max)
					b1Max = proj1;

				if (proj2 < b2Min)
					b2Min = proj2;
				if (proj2 > b2Max)
					b2Max = proj2;
			}
		}

		float midPoint = (b1Max + b1Min) * 0.5f;
		float dist = b1Max - midPoint;

		if ((midPoint < b2Min || midPoint > b2Max) || abs(midPoint - b2Min) > dist && abs(midPoint - b2Max) > dist) {

			return false;
		}
	}

	return true;
}

inline bool ColliderColliderCheck(Collider* c1, Collider* c2, PhysCollisionInfo& colInfo) {

	if(c1->GetType() == Collider::ColliderType::Circle && c2->GetType() == Collider::ColliderType::Circle)
		return CircleCircleCollision(static_cast<CircleCollider*>(c1), static_cast<CircleCollider*>(c2), colInfo);

	if (c1->GetType() == Collider::ColliderType::Box && c2->GetType() == Collider::ColliderType::Circle)
		return BoxCircleCollision(static_cast<BoxCollider*>(c1), static_cast<CircleCollider*>(c2), colInfo);

	if (c1->GetType() == Collider::ColliderType::Circle && c2->GetType() == Collider::ColliderType::Box)
		return BoxCircleCollision(static_cast<BoxCollider*>(c2), static_cast<CircleCollider*>(c1), colInfo);

	if (c1->GetType() == Collider::ColliderType::Box && c2->GetType() == Collider::ColliderType::Box)
		return BoxBoxCollision(static_cast<BoxCollider*>(c1), static_cast<BoxCollider*>(c2), colInfo);

	return false;
}

#endif // _PHYSICSHELPER_H
