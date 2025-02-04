#include "NodeActor.h"
#include "Engine/Engine.h"
#include "Manager/SoundManager.h"
#include "Level/InGameLevel.h"

NodeActor::NodeActor(const Vector2& position, NodePositionType type, InGameLevel* ingameLevel)
	:DrawableActor("@@@@@@@@@@")
{
	this->ingameLevel = ingameLevel;
	this->position = position;
	yPosition = (float)position.y;
	nodePosType = type;
	
	if (type == NodePositionType::MidleLeft || type == NodePositionType::MidleRight)
	{
		color = Color::Blue;
	}
	else
	{
		color = Color::Green;
	}
}

NodeActor::~NodeActor()
{
	//if (node != nullptr)
	//{
	//	delete node;
	//}
}

void NodeActor::Update(float deltaTime)
{
	++test;
	Super::Update(deltaTime);

	//이동 로직.
	yPosition += speed * deltaTime;
	position.y = (int)yPosition;

	//화면 벗어나면 제거.
	if (yPosition >= Engine::Get().ScreenSize().y -10)
	{
		Destroy();
		
		if (ingameLevel != nullptr)
		{
			ingameLevel->ComboReset();
		}
	}
}

void NodeActor::Draw()
{
	Super::Draw();
}

void NodeActor::Hit()
{
	Destroy();
}
