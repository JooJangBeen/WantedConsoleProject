#include "LevelManager.h"
#include "Level/CreateNodeLevel.h"
#include "Level/MenuLevel.h"
#include "Level/LobbyLevel.h"
#include "Level/InGameLevel.h"
#include "Engine/Engine.h"

LevelManager* LevelManager::instance = nullptr;

LevelManager::LevelManager()
{
	instance = this;

	createNodeLevel = new CreateNodeLevel("Magnolia");
	menuLevel = new MenuLevel();
	lobbyLevel = new LobbyLevel();
	ingameLevel = new InGameLevel();
}	

LevelManager::~LevelManager()
{
	if (menuLevel != nullptr) delete menuLevel;
	if (createNodeLevel != nullptr) delete createNodeLevel;
	if (lobbyLevel != nullptr) delete lobbyLevel;
	if (ingameLevel != nullptr) delete ingameLevel;
}

Level** LevelManager::GetTargetLevel(LevelType levelTtpe)
{
	switch (levelTtpe)
	{
		case LevelType::MenuLevel:
		{
			return &menuLevel;
		}
		case LevelType::CreateNodeLevel:
		{
			return &createNodeLevel;
		}
		case LevelType::LobbyLevel:
		{
			return &lobbyLevel;
		}
		case LevelType::IngameLevel:
		{
			return &ingameLevel;
		}
		default:
			return nullptr;
	}
}

void LevelManager::ChangeMainLevel(LevelType levelTtpe)
{
	system("cls");

	switch (levelTtpe)
	{
		case LevelType::MenuLevel:
		{
			if (currentLevel != nullptr)
			{
				currentLevel->ClearActors();
			}

			menuLevel->Init();
			Engine::Get().LoadLevel(menuLevel);
			currentLevel = menuLevel;
			break;
		}
		case LevelType::CreateNodeLevel:
		{
			if (currentLevel != nullptr)
			{
				currentLevel->ClearActors();
			}

			createNodeLevel->Init();
			Engine::Get().LoadLevel(createNodeLevel);
			currentLevel = createNodeLevel;
			break;
		}
		case LevelType::LobbyLevel:
		{
			if (currentLevel != nullptr)
			{
				currentLevel->ClearActors();
			}

			lobbyLevel->Init();
			Engine::Get().LoadLevel(lobbyLevel);
			currentLevel = lobbyLevel;
			break;
		}
		case LevelType::IngameLevel:
		{
			if (currentLevel != nullptr)
			{
				currentLevel->ClearActors();
			}

			if (lobbyLevel->As<LobbyLevel>() != nullptr)
			{
				ingameLevel->As<InGameLevel>()->Init(lobbyLevel->As<LobbyLevel>()->GetSelectMusicName());
			}
			else
			{
				ingameLevel->Init();
			}

			Engine::Get().LoadLevel(ingameLevel);
			currentLevel = ingameLevel;
			break;
		}
		default:
			break;
	}
}
