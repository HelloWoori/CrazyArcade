// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
//디버깅용(윈도우창과 동시에 콘솔창 띄우기)
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cassert>
#include <map>
#include <string>
#include <functional>
using namespace std;

// 내가 추가한 헤더
#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "renderer.h"
#include "enum.h"
#include "struct.h"
#include "define.h"

#include "keyManager.h"
#include "imageManager.h"
#include "timeManager.h"
#include "gameObjectManager.h"
#include "sceneManager.h"
#include "gameStateManager.h"
#include "soundManager.h"

#include "tile.h"
#include "tileFactory.h"

//게임 오브젝트 들
#include "player.h"
#include "bomb.h"
#include "designElement.h"
#include "wave.h"
#include "waveStartingPoint.h"
#include "item.h"

// 싱글톤
#define KEYMANAGER KeyManager::getSingleton()
#define RANDOM RandomFunction::getSingleton()
#define GAMEOBJMANGER GameObjectManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define TIMEMANAGER TimeManager::getSingleton()
#define SCENEMANAGER SceneManager::getSingleton()
#define GAMESTATEMANAGER GameStateManager::getSingleton()
#define SOUNDMANAGER SoundManager::getSingleton()


// define
#define WINNAME (LPTSTR)(TEXT("Crazy Arcade"))
#define WINSTARTX    200
#define WINSTARTY    30
#define WINSIZEX     1040 //<- 800
#define WINSIZEY     780  //<- 600
#define CENTERX      (WINSIZEX / 2)
#define CENTERY      (WINSIZEY / 2)
#define WINSTYLE WS_CAPTION | WS_SYSMENU

// SAFE DELETE
#define SAFE_DELETE(p)          {if(p)  {delete   (p);   (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p)    {if(p)  {delete[] (p);   (p)=NULL;}}
#define SAFE_RELEASE(p)         {if(p)  {(p)->release(); (p)=NULL;}}


// extern: 컴파일에게 해당 변수가 외부에 선언되어있음을 알리는 것
extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT ptMouse;