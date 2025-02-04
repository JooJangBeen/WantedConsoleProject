#include <iostream>
#include "Engine/Engine.h"
#include "Game/Game.h"
#include <Windows.h>
#include "Level/MenuLevel.h"
#include "Manager/SoundManager.h"
#include "Manager/LevelManager.h"
#include "Actor/DrawableActor.h"
#include "Actor/NodeActor.h"

// 종료 핸들러 함수
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT) {
        std::cout << "Console is being closed. Cleaning up resources...\n";
        Sleep(2000); // 테스트: 리소스 정리 시간 확보
    }
    return TRUE;
}


int main()
{
	CheckMemoryLeak();
	Game game;
    SoundManager soundManager;
    LevelManager levelManager;

    levelManager.Get().ChangeMainLevel(LevelType::MenuLevel);
    game.Run();

    ////LevelManager* test = new LevelManager();
    ////LevelManager* test2 = new LevelManager[3];

    //NodeActor* testActor = new NodeActor(Vector2(1, 2), NodePositionType::Left);
    //NodeActor* testActor2 = new NodeActor(Vector2(1, 2), NodePositionType::Left);
    //NodeActor* testActor3= new NodeActor(Vector2(1, 2), NodePositionType::Left);
    //NodeActor* testActor4 = new NodeActor(Vector2(1, 2), NodePositionType::Left);

    //List<DrawableActor*> test;
    //test.PushBack(testActor);
    //test.PushBack(testActor2);
    //test.PushBack(testActor3);
    //test.PushBack(testActor4);

    //delete testActor2;
    //delete testActor3;
    //delete testActor4;
    //delete testActor;

}