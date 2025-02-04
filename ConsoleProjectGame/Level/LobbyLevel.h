#pragma once
#include "Level/Level.h"

enum class MusicName
{
	None = -1,
	Onlyforyou = 0,
	EndoftheMoonlight,
	Magnolia,
};

struct LobbyMenuItem
{
	using OnSelected = void (*)();

	LobbyMenuItem(MusicName musicName, const char* text, OnSelected onSelected)
	{
		size_t length = strlen(text);
		menuText = new char[length + 1];
		strcpy_s(menuText, length + 1, text);

		this->musicName = musicName;

		this->onSelected = onSelected;
	}

	~LobbyMenuItem()
	{
		delete[] menuText;
	}

	char* menuText;
	OnSelected onSelected;
	MusicName musicName = MusicName::None;
};

struct MenuItem;
class LobbyLevel : public Level
{
	RTTI_DECLARATIONS(LobbyLevel, Level)

public:
	LobbyLevel();
	~LobbyLevel();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	inline MusicName& GetSelectMusicName() { return musicName; }

private:
	int currentIndex = 0;
	Color selectedColor = Color::Red;
	Color unselectedColor = Color::White;
	MusicName musicName = MusicName::None;

	List<LobbyMenuItem*> lobbyMenuItems;
	int length = 0;
};