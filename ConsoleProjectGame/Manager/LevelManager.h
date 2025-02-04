#pragma once


enum class LevelType
{
	None = -1,
	MenuLevel = 0,
	CreateNodeLevel,
	LobbyLevel,
	IngameLevel,
};

class Level;
class LevelManager
{
public:
	LevelManager();
	~LevelManager();


public:
	static LevelManager& Get() { return *instance; }

	Level** GetTargetLevel(LevelType levelTtpe);
	void ChangeMainLevel(LevelType levelTtpe);


private:
	static LevelManager* instance;

	Level* currentLevel = nullptr;

	Level* menuLevel = nullptr;
	Level* createNodeLevel = nullptr;
	Level* lobbyLevel = nullptr;
	Level* ingameLevel = nullptr;

};