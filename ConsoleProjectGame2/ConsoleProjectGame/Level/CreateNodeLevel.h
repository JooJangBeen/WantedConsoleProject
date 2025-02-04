#pragma once
#include "Level/Level.h"
#include <vector>
#include <string>
#include <fmod.hpp>


struct Node;
enum class NodePositionType;
enum class NodeType;
class NodeActor;

using FMOD::Sound;
using FMOD::Channel;

class CreateNodeLevel : public Level
{
	RTTI_DECLARATIONS(CreateNodeLevel, Level)

public:
	CreateNodeLevel(std::string fileName);
	~CreateNodeLevel();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	void CreatePlayStart();
	void CreatePlayEnd();
	void CreateNode(NodeType nodeType, NodePositionType nodePositionType, float createTime);
	void TestPlay();
	void SaveNode();


private:
	std::vector<Node*> nodeVector;
	float cumulativeTime = 0.0f;
	unsigned int currentPlaySoundPos = 0;
	unsigned int playSoundLength = 0;

	bool isCreatStart = false;
	bool isTestStart = false;
	Sound* playSound = nullptr;
	Channel* playChannel = nullptr;
	std::string soundFileName = "";
};