#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Container/List.h"
#include <vector>

// ���� ����(Forward Declaration).
class Actor;
class ENGINE_API Level : public RTTI
{
	// RTTI ����.
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	virtual void Init();

	// ���� �߰� �Լ�.
	void AddActor(std::vector<Actor*> actorVec);
	void ClearActors();

	// ���� ��û�� �� ���͸� �����ϴ� �Լ�.
	//void DestroyActor();
	void ProcessAddedAndDestroyedActor();

	// ���� ó�� �Լ�.
	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	// ���� ������ ��ġ�Ǵ� ��ü(����) �迭.
	List<Actor*> actors;

	// �߰� ��û�� ����.
	std::vector<Actor*> addRequestedActorVec;
};