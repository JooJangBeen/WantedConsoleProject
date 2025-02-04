#include "MenuLevel.h"
#include "Game/Game.h"
#include "Manager/LevelManager.h"

MenuLevel::MenuLevel()
{
	menuItems.PushBack(new MenuItem("CreateNode", []() {LevelManager::Get().ChangeMainLevel(LevelType::CreateNodeLevel);}));
	menuItems.PushBack(new MenuItem("Lobby", []() {LevelManager::Get().ChangeMainLevel(LevelType::LobbyLevel);}));
	menuItems.PushBack(new MenuItem("Quit Game", []() { Game::Get().QuitGame(); }));
	length = menuItems.Size();
}

MenuLevel::~MenuLevel()
{
	for (auto* item : menuItems)
	{
		if (item == nullptr) continue;
		delete item;
		item = nullptr;
	}
}

void MenuLevel::Init()
{
	currentIndex = 0;
}

void MenuLevel::Update(float deltaTime)
{
	/*if (Game::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleMenu();
	}*/

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
		menuItems[currentIndex]->onSelected();
	}
}

void MenuLevel::Draw()
{
	Super::Draw();

	Engine::Get().Draw(Vector2(0, 0), "MenuLevel");

	for (int ix = 0; ix < length; ++ix)
	{
		Engine::Get().Draw(Vector2(0, 2 + ix), menuItems[ix]->menuText, ix == currentIndex ? selectedColor : unselectedColor);
	}
}