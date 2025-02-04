#include "LobbyLevel.h"
#include "Manager/LevelManager.h"
#include "Level/MenuLevel.h"
#include "Game/Game.h"

LobbyLevel::LobbyLevel()
{
	lobbyMenuItems.PushBack(new LobbyMenuItem(MusicName::Onlyforyou,"Onlyforyou", []() {LevelManager::Get().ChangeMainLevel(LevelType::IngameLevel);}));
	lobbyMenuItems.PushBack(new LobbyMenuItem(MusicName::Magnolia, "Magnolia", []() {LevelManager::Get().ChangeMainLevel(LevelType::IngameLevel);}));
	lobbyMenuItems.PushBack(new LobbyMenuItem(MusicName::EndoftheMoonlight, "EndoftheMoonlight", []() {LevelManager::Get().ChangeMainLevel(LevelType::IngameLevel);}));
	lobbyMenuItems.PushBack(new LobbyMenuItem(MusicName::None,"Return Lobby", []() {LevelManager::Get().ChangeMainLevel(LevelType::MenuLevel);}));

	length = lobbyMenuItems.Size();
}

LobbyLevel::~LobbyLevel()
{
	for (auto* item : lobbyMenuItems)
	{
		if (item == nullptr) continue;
		delete item;
		item = nullptr;
	}
}

void LobbyLevel::Init()
{
	currentIndex = 0;
}

void LobbyLevel::Update(float deltaTime)
{
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		LevelManager::Get().ChangeMainLevel(LevelType::MenuLevel);
	}
	if (Game::Get().GetKeyDown(VK_UP))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}
	if (Game::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	if (Game::Get().GetKeyDown(VK_RETURN))
	{
		musicName = lobbyMenuItems[currentIndex]->musicName;
		lobbyMenuItems[currentIndex]->onSelected();
	}
}

void LobbyLevel::Draw()
{
	Super::Draw();

	Engine::Get().Draw(Vector2(0, 0), "LobbyLevel");

	for (int ix = 0; ix < length; ++ix)
	{
		Engine::Get().Draw(Vector2(0, 2 + ix), lobbyMenuItems[ix]->menuText, ix == currentIndex ? selectedColor : unselectedColor);
	}
}
