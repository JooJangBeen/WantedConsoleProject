#include "InGameLevel.h"
#include "Engine/Engine.h"
#include "Manager/LevelManager.h"
#include "Level/LobbyLevel.h"
#include "Actor/NodeActor.h"
#include "Manager/SoundManager.h"

InGameLevel::InGameLevel()
{
	rows = Engine::Get().ScreenSize().y - 10;
	cols = Engine::Get().ScreenSize().x - 10;
	hitMax = rows - 1;
	hitMin = rows - 4;
	perfectPosY = rows - 3;

	inGameMap = new char* [rows];

	for (int i = 0; i < rows; ++i)
	{
		inGameMap[i] = new char[cols];
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			inGameMap[i][j] = ' '; // 공백 문자로 초기화
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (j > cols - 29) break;

			if (i == 0 || i == rows-2 || i == rows - 1)
			{ // 첫 번째 행
				if (i == rows - 2)
				{
					inGameMap[i][j] = '=';
				}
				else
				{
					inGameMap[i][j] = '-';
				}
			}
			else  if (j == 0 || j == 20 || j == 40 || j == 60 || j == 80)
			{
				inGameMap[i][j] = 'l';
			}
			else
			{
				inGameMap[i][j] = ' ';
			}
		}
	}
}

InGameLevel::~InGameLevel()
{
	while (!nodeQueue.empty())
	{
		Node* node = nodeQueue.front(); // 큐의 맨 앞 요소 가져오기
		delete node;                    // 메모리 해제
		nodeQueue.pop();                // 큐에서 제거
	}

	for (int i = 0; i < rows; i++) {
		delete[] inGameMap[i];
	}
	delete inGameMap;
}

void InGameLevel::Init()
{
}

void InGameLevel::Update(float deltaTime)
{
	//if (isFirst)
	//{
	//	if (TempCount <= 10)
	//	{
	//		++TempCount;
	//		return;
	//	}
	//	else
	//	{
	//		isFirst = false;
	//	}

	//}
	if (deltaTime <= 0.0f || deltaTime > 0.5f)
	{
		return;
	}

	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		PlayEnd();
		LevelManager::Get().ChangeMainLevel(LevelType::LobbyLevel);
		return;
	}

	if (isEnd) return;

	if (!isStart)
	{
		startTime -= deltaTime;

		if (startTime < 0.0f)
		{
			isStart = true;

			playMusicSound = SoundManager::Get().FPlaySound(musicName, SoundType::BackGround, FMOD_LOOP_OFF);

			if (playMusicSound != nullptr)
			{
				playMusicSound->getLength(&playSoundLength, FMOD_TIMEUNIT_MS);
			}
			playChannel = SoundManager::Get().GetChannel(SoundType::BackGround);
		}
		else
		{
			return;
		}
	}

	cumulativeTime += deltaTime;
	accuracyTime += deltaTime;
	comboTime += deltaTime;
	isSameNote = false;
	++testtttt;
	
	//해당 채널 노래의 현재 진생 상황을 가지고 오는 함수.
	playChannel->getPosition(&currentPlaySoundPos, FMOD_TIMEUNIT_MS);

	if (currentPlaySoundPos >= playSoundLength)
	{
		isEnd = true;
	}

	CreateNodeActor(cumulativeTime);

	//if (!isChecked && cumulativeTime > 3.0f)//TEST
	//{
	//	isChecked = true;
	//	__debugbreak();
	//}

	if (Engine::Get().GetKeyDown(0x44))//D
	{
		CheckHit(NodePositionType::Left);
	}
	if (Engine::Get().GetKeyDown(0x46))//F
	{
		CheckHit(NodePositionType::MidleLeft);
	}
	if (Engine::Get().GetKeyDown(0x4A))//J
	{
		CheckHit(NodePositionType::MidleRight);
	}
	if (Engine::Get().GetKeyDown(0x4B))//K
	{
		CheckHit(NodePositionType::Right);
	}

	Super::Update(deltaTime);
}

void InGameLevel::Draw()
{
	char temp;

	if (isEnd)
	{
		perfectString = "//PERFECT : " + std::to_string(perfectCount) + "//";
		goodString = "//GOOD : " + std::to_string(goodCount) + "//";
		oopsString = "//OOPS : " + std::to_string(oopsCount) + "//";
		Engine::Get().Draw(Vector2(30, 7), perfectString.c_str(), Color::Yellow);
		Engine::Get().Draw(Vector2(30, 8), goodString.c_str(), Color::Green);
		Engine::Get().Draw(Vector2(30, 9), oopsString.c_str(), Color::Red);
		Engine::Get().Draw(Vector2(30, 10), scoreString.c_str(), Color::Blue);
		return;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols - 1; j++)
		{
			temp = inGameMap[i][j];
			Engine::Get().Draw(i, j, temp);
		}
	}

	if (!isStart)
	{
		startString = "//READY : " + std::to_string((int)startTime) + "//";
		Engine::Get().Draw(Vector2(30, 10), startString.c_str(), Color::Green);
		return;
	}

	Engine::Get().Draw(Vector2(cols - 20, 7),(std::string("remaining Time: ") + std::to_string(playSoundLength - currentPlaySoundPos)).c_str(), Color::Green);
	Engine::Get().Draw(Vector2(cols - 20, 10), scoreString.c_str(), Color::Red);

	if (comboTime < 2.5f)
	{
		Engine::Get().Draw(Vector2(cols - 20, 9), comboString.c_str(), Color::Green);
	}

	if (accuracyTime < 0.15f)
	{
		ShowAccuracyString();
	}

	Super::Draw();
}

void InGameLevel::Init(MusicName musicNameType)
{
	Engine::Get().StopUpdate();
	isChecked = false;
	testtttt = 0;

	Init();
	this->musicNameType = musicNameType;
	accuracyTime = 1.0f;
	comboTime = 1.0f;
	cumulativeTime = 0.0f;
	currentPlaySoundPos = 0;
	playSoundLength = 0;
	combo = 0;
	scoreString = "";
	comboString = "";
	isStart = false;
	isEnd = false;
	startTime = 4.0f;
	perfectCount = 0;
	goodCount = 0;
	oopsCount = 0;
	perfectString = "";
	goodString = "";
	oopsString = "";

	switch (musicNameType)
	{
	case MusicName::Onlyforyou:
	{
		musicName = "Onlyforyou";
		break;
	}
	case MusicName::EndoftheMoonlight:
	{
		musicName = "EndoftheMoonlight";
		break;
	}
	case MusicName::Magnolia:
	{
		musicName = "Magnolia";
		break;
	}
	default:
		musicName = " ";
		break;
	}


	LoadData();

	Engine::Get().StartUpdate();

	//playMusicSound = SoundManager::Get().FPlaySound(musicName, SoundType::BackGround, FMOD_LOOP_OFF);

	//if (playMusicSound != nullptr)
	//{
	//	playMusicSound->getLength(&playSoundLength, FMOD_TIMEUNIT_MS);
	//}
	//playChannel = SoundManager::Get().GetChannel(SoundType::BackGround);
}

void InGameLevel::LoadData()
{
	int nodeType = 0;
	int nodePositionType = 0;
	float createTime = 0.0f;

	std::string fullPath = "../Assets/NodeData/" + musicName + "Data.txt";
	FILE* file = nullptr;
	fopen_s(&file, fullPath.c_str(), "rt");
	char buffer[256] = {};

	if (file != nullptr)
	{
		while (true)
		{
			if (fgets(buffer, sizeof(buffer), file) == nullptr)
			{
				break;
			}
			sscanf_s(buffer, "nodeType: %d, nodePositionType: %d, createTime: %f", &nodeType, &nodePositionType, &createTime);

			nodeQueue.emplace(new Node((NodeType)nodeType, (NodePositionType)nodePositionType, createTime));
		}

		fclose(file);
	}
}

void InGameLevel::CreateNodeActor(float cumulativeTime)
{
	Vector2 tempVec;
	
	int test = Engine::Get().ScreenSize().y;

	while (!nodeQueue.empty())
	{
		Node* node = nodeQueue.front(); // 큐의 맨 앞 요소 가져오기

		if (node->createTime - ((Engine::Get().ScreenSize().y - 10) / 25.0f) <= cumulativeTime)
		{
			switch (node->eNodePositionType)
			{
			case NodePositionType::Left:
			{
				tempVec = Vector2(5, 0);
				break;
			}
			case NodePositionType::MidleLeft:
			{
				tempVec = Vector2(25, 0);
				break;
			}
			case NodePositionType::MidleRight:
			{
				tempVec = Vector2(45, 0);
				break;
			}
			case NodePositionType::Right:
			{
				tempVec = Vector2(65, 0);
				break;
			}
			}
			actors.PushBack(new NodeActor(tempVec, node->eNodePositionType, this));

			nodeQueue.pop();                // 큐에서 제거
			delete node;                    // 메모리 해제
		}
		else
		{
			return;
		}
	}
}

void InGameLevel::CheckHit(NodePositionType posType)
{
	for (Actor* actor : actors)
	{
		if (actor == nullptr || !actor->IsAcive()) continue;

		if (actor->As<NodeActor>()->Position().y >= hitMin && actor->As<NodeActor>()->Position().y <= hitMax)
		{
			if (actor->As<NodeActor>()->GetNodePositionType() == posType)
			{
				accuracyTime = 0.0f;
				comboTime = 0.0f;
				if (!isSameNote)
				{
					SoundManager::Get().FPlaySound("NoteSoundDon", SoundType::EffectSound, FMOD_LOOP_OFF);
				}
				isSameNote = true;

				actor->As<NodeActor>()->Hit();
				currentAccuracy = abs(perfectPosY - actor->Position().y);
				if (currentAccuracy == 0)
				{
					++perfectCount;
					score += 30;
				}
				else if(currentAccuracy == 1)
				{
					++goodCount;
					score += 20;
				}
				else
				{
					++oopsCount;
					score += 10;
				}
				++combo;
				scoreString = "//Score : " + std::to_string(score) + "//";
				comboString = "//COMBO : " + std::to_string(combo) + "//";
			}
		}
	}
}

void InGameLevel::PlayEnd()
{
	SoundManager::Get().AllStop();
	//playChannel = SoundManager::Get().GetChannel(SoundType::BackGround);
	/*if (playChannel)
	{
		playChannel->stop();
	}*/

	while (!nodeQueue.empty())
	{
		Node* node = nodeQueue.front(); // 큐의 맨 앞 요소 가져오기
		delete node;                    // 메모리 해제
		nodeQueue.pop();                // 큐에서 제거
	}
}

void InGameLevel::ShowAccuracyString()
{
	if (currentAccuracy == 0)
	{
		Engine::Get().Draw(Vector2(10, 21), "@@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 22), "@@      @ @@        @@      @ @         @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 23), "@@      @ @@        @@      @ @         @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 24), "@@@@@@@@@ @@        @@@@@@@@@ @@@@@@@@@ @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 25), "@@        @@@@@@@@@ @@@       @         @@@@@@@@@ @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 26), "@@        @@        @@ @      @         @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 27), "@@        @@        @@  @     @         @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 28), "@@        @@        @@   @    @         @@        @@            @@   ", Color::Yellow);
		Engine::Get().Draw(Vector2(10, 29), "@@        @@@@@@@@@ @@    @   @         @@@@@@@@@ @@@@@@@@@     @@   ", Color::Yellow);

	}
	else if (currentAccuracy == 1)
	{
		Engine::Get().Draw(Vector2(10, 21), "@@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@                              ", Color::Green);
		Engine::Get().Draw(Vector2(10, 22), "@@@     @ @@     @@ @@     @@ @@      @@                             ", Color::Green);
		Engine::Get().Draw(Vector2(10, 23), "@@@     @ @@     @@ @@     @@ @@       @@                            ", Color::Green);
		Engine::Get().Draw(Vector2(10, 24), "@@		   @@     @@ @@     @@ @@        @@                           ", Color::Green);
		Engine::Get().Draw(Vector2(10, 25), "@@        @@     @@ @@     @@ @@        @@                           ", Color::Green);
		Engine::Get().Draw(Vector2(10, 26), "@@   @@@@ @@     @@ @@     @@ @@        @@                           ", Color::Green);
		Engine::Get().Draw(Vector2(10, 27), "@@     @@ @@     @@ @@     @@ @@       @@                            ", Color::Green);
		Engine::Get().Draw(Vector2(10, 28), "@@     @@ @@     @@ @@     @@ @@      @@                             ", Color::Green);
		Engine::Get().Draw(Vector2(10, 29), "@@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@                              ", Color::Green);
	}
	else
	{
		Engine::Get().Draw(Vector2(10, 21), "@@@@@@@@@ @@@@@@@@@ @@@@@@@@@ @@@@@@@@@                              ", Color::Red);
		Engine::Get().Draw(Vector2(10, 22), "@@     @@ @@     @@ @@      @ @@                                     ", Color::Red);
		Engine::Get().Draw(Vector2(10, 23), "@@     @@ @@     @@ @@      @ @@                                     ", Color::Red);
		Engine::Get().Draw(Vector2(10, 24), "@@     @@ @@     @@ @@@@@@@@@ @@                                     ", Color::Red);
		Engine::Get().Draw(Vector2(10, 25), "@@     @@ @@     @@ @@        @@@@@@@@@                              ", Color::Red);
		Engine::Get().Draw(Vector2(10, 26), "@@     @@ @@     @@ @@               @@                              ", Color::Red);
		Engine::Get().Draw(Vector2(10, 27), "@@     @@ @@     @@ @@               @@                              ", Color::Red);
		Engine::Get().Draw(Vector2(10, 28), "@@     @@ @@     @@ @@               @@                              ", Color::Red);
		Engine::Get().Draw(Vector2(10, 29), "@@@@@@@@@ @@@@@@@@@ @@        @@@@@@@@@                              ", Color::Red);
	}
}
