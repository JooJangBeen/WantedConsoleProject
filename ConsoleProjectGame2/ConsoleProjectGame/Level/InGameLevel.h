#pragma once
#include "Level/Level.h"
#include <string>
#include <queue>
#include <fmod.hpp>


using FMOD::Sound;
using FMOD::Channel;
enum class MusicName;
struct Node;
enum class NodePositionType;
class InGameLevel : public Level
{
	RTTI_DECLARATIONS(InGameLevel, Level)

public:
	InGameLevel();
	~InGameLevel();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	void Init(MusicName musicNameType);
	inline void ComboReset() { combo = 0; };

	void operator()()
	{
		ComboReset();
	}

private:
	void LoadData();
	void CreateNodeActor(float cumulativeTime);
	void CheckHit(NodePositionType posType);
	void PlayEnd();

	void ShowAccuracyString();

private:
	std::queue<Node*> nodeQueue;

	std::string musicName = "";
	std::string scoreString = "";
	std::string comboString = "";
	std::string startString = "";
	MusicName musicNameType;
	int rows = 0; //행
	int cols = 0; //열
	int hitMin = 0;
	int hitMax = 0;
	int perfectPosY = 0;
	char** inGameMap = nullptr;

	float cumulativeTime = 0.0f;
	unsigned int currentPlaySoundPos = 0;
	unsigned int playSoundLength = 0;
	Sound* playMusicSound = nullptr;
	Channel* playChannel = nullptr;

	//시작/끝 관련.
	bool isStart = false;
	bool isEnd = false;
	float startTime = 4.0f;
	int perfectCount = 0;
	int goodCount = 0;
	int oopsCount = 0;
	std::string perfectString = "";
	std::string goodString = "";
	std::string oopsString = "";

	//정확도관련.
	float accuracyTime = 1.0f;
	int currentAccuracy = 0;
	bool isSameNote = false;

	//콤보 관련.
	int combo = 0;
	float comboTime = 1.0f;

	//점수.
	int score = 0;

	bool isChecked = false;

	//임시
	bool isFirst = true;
	int TempCount = 0;
	int testtttt = 0;
};