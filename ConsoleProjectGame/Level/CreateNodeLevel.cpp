#include "CreateNodeLevel.h"
#include "Game/Game.h"
#include "Manager/SoundManager.h"
#include "Manager/LevelManager.h"
#include "Actor/NodeActor.h"


CreateNodeLevel::CreateNodeLevel(std::string fileName)
	:soundFileName(fileName)
{
}

CreateNodeLevel::~CreateNodeLevel()
{
	if (nodeVector.size() != 0)
	{
		for (int i = 0; i < nodeVector.size(); ++i)
		{
			delete nodeVector[i];
		}

		nodeVector.clear();
	}
}

void CreateNodeLevel::Init()
{
	cumulativeTime = 0.0f;
	currentPlaySoundPos = 0;
	playSoundLength = 0;
	isCreatStart = false;
	isTestStart = false;
	playSound = nullptr;
	playChannel = nullptr;
}

void CreateNodeLevel::Update(float deltaTime)
{
	if (!isCreatStart && !isTestStart)
	{
		if(Engine::Get().GetKeyDown(VK_ESCAPE))
		{
			LevelManager::Get().ChangeMainLevel(LevelType::MenuLevel);
		}

		if (Engine::Get().GetKeyDown(VK_RETURN))
		{
			SaveNode();
		}
	}

	if (isCreatStart && !isTestStart && playSound)
	{
		if (playChannel)
		{
			//해당 채널 노래의 현재 진생 상황을 가지고 오는 함수.
			playChannel->getPosition(&currentPlaySoundPos, FMOD_TIMEUNIT_MS);

			//노래가 끝났는지를 체크.
			if (currentPlaySoundPos >= playSoundLength)
			{
				CreatePlayEnd();
			}
		}
	}

	if (Engine::Get().GetKeyDown(VK_SPACE) && !isCreatStart)
	{
		CreatePlayStart();
	}

	if (isCreatStart)
	{
		if (Engine::Get().GetKeyDown(VK_ESCAPE))
		{
			CreatePlayEnd();

			if (nodeVector.size() != 0)
			{
				for (int i = 0; i < nodeVector.size(); ++i)
				{
					delete nodeVector[i];
				}

				nodeVector.clear();
			}
		}

		cumulativeTime += deltaTime;

		if (Engine::Get().GetKeyDown(0x44))//D
		{
			CreateNode(NodeType::NormalNode, NodePositionType::Left, cumulativeTime);
		}
		if (Engine::Get().GetKeyDown(0x46))//F
		{
			CreateNode(NodeType::NormalNode, NodePositionType::MidleLeft, cumulativeTime);
		}
		if (Engine::Get().GetKeyDown(0x4A))//J
		{
			CreateNode(NodeType::NormalNode, NodePositionType::MidleRight, cumulativeTime);
		}
		if (Engine::Get().GetKeyDown(0x4B))//K
		{
			CreateNode(NodeType::NormalNode, NodePositionType::Right, cumulativeTime);
		}
	}

}

void CreateNodeLevel::Draw()
{
	if (isCreatStart)
	{
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x /2, Engine::Get().ScreenSize().y / 2), "Play Creating node!!!", Color::Green);
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x / 2, (Engine::Get().ScreenSize().y / 2) + 1),
			(std::string("remaining Time: ") + std::to_string(playSoundLength - currentPlaySoundPos)).c_str(), Color::Green);
	}
	else if (!isCreatStart && !isTestStart)
	{
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x / 2, Engine::Get().ScreenSize().y / 2), "Waiting!!", Color::Red);
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x / 2, (Engine::Get().ScreenSize().y / 2)+1), "Play To Create Node or Test", Color::Red);
	}
}

void CreateNodeLevel::CreatePlayStart()
{
	isCreatStart = true;
	playSound = SoundManager::Get().FPlaySound(soundFileName, SoundType::BackGround, FMOD_LOOP_OFF);

	if (playSound != nullptr)
	{
		playSound->getLength(&playSoundLength, FMOD_TIMEUNIT_MS);
	}
	playChannel = SoundManager::Get().GetChannel(SoundType::BackGround);
}

void CreateNodeLevel::CreatePlayEnd()
{
	isCreatStart = false;

	playChannel = SoundManager::Get().GetChannel(SoundType::BackGround);

	if (playChannel)
	{
		playChannel->stop();
	}

	Init();

}

void CreateNodeLevel::CreateNode(NodeType nodeType, NodePositionType nodePositionType, float createTime)
{
	nodeVector.emplace_back(new Node(nodeType, nodePositionType, createTime));
}

void CreateNodeLevel::TestPlay()
{
}

void CreateNodeLevel::SaveNode()
{
	FILE* file = nullptr;
	char textBuffer[256];

	std::string fullPath = "../Assets/NodeData/" + soundFileName + "Data.txt";

	fopen_s(&file, fullPath.c_str(), "wt");

	if (file != nullptr)
	{
		if (nodeVector.size() != 0)
		{
			for (int i = 0; i < nodeVector.size(); ++i)
			{
				if (nodeVector[i] == nullptr) continue;

				snprintf(textBuffer, sizeof(textBuffer), "nodeType: %d, nodePositionType: %d, createTime: %f \n",
					(int)nodeVector[i]->eNodeType, (int)nodeVector[i]->eNodePositionType, nodeVector[i]->createTime);

				fputs(textBuffer, file);
			}
		}

		fclose(file);
	}
}
