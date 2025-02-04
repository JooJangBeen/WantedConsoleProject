#pragma once
#include "Actor/DrawableActor.h"
#include <vector>
#include <string>


enum class NodeType
{
	None = -1,
	NormalNode = 0,
	PressNode,
};

enum class NodePositionType
{
	None = -1,
	Left = 0,
	MidleLeft,
	MidleRight,
	Right,
};

struct Node
{
	Node(NodeType nodeType, NodePositionType nodePositionType, float createTime)
	{
		eNodeType = nodeType;
		this->eNodePositionType = nodePositionType;
		this->createTime = createTime;
	}

	NodeType eNodeType = NodeType::None;
	NodePositionType eNodePositionType = NodePositionType::None;
	float createTime = 0.0f;
};

class InGameLevel;
class NodeActor : public DrawableActor
{
	//using OnResetCombo = std::function<void()>;
	//using OnResetCombo = void (*)();
	RTTI_DECLARATIONS(NodeActor, DrawableActor)

public:
	NodeActor(const Vector2& position, NodePositionType type, InGameLevel* ingameLevel);
	~NodeActor();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	inline NodePositionType& GetNodePositionType() { return this->nodePosType; }
	void Hit();
	int GetTestCount() { return test; }


private:
	float yPosition = 0.0f;
	float speed = 25.0f;
	Vector2 tempVector;
	NodePositionType nodePosType;
	InGameLevel* ingameLevel = nullptr;
	int test = 0;
	//OnResetCombo onResetCombo;

};