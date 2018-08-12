#include "stdafx.h"
#include "sceneManager.h"
#include "loginScene.h"
#include "modeSelectScene.h"
#include "lobbyScene.h"
#include "playScene.h"
SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{

    /* 씬 등록 */
    LoginScene* login = new LoginScene();
    SCENEMANAGER->registerScene(SceneTag::Login, login);

    ModeSelectScene* modeSelect = new ModeSelectScene;
    SCENEMANAGER->registerScene(SceneTag::ModeSelect, modeSelect);

	//로비씬은 모드선택씬에서 로비씬으로 넘어가는 순간 생성으로 변경
	//플레이씬은 로비씬에서 플레이씬으로 넘어가는 순간 생성으로 변경

    /* 가장 첫 씬 설정 */
    _currentScene = login;
    initCurrentScene(); //이거땜에 오류나는거여따 ㅠㅠ
}

//해제
void SceneManager::release()
{
    deleteAll();
}

//씬을 등록한다
void SceneManager::registerScene(SceneTag tag, Scene* scene)
{
    _mSceneList.insert(make_pair(tag, scene));
}


void SceneManager::changeScene(SceneTag tag)
{
    //넘어온 씬이 존재한다면
    if (auto scene = findScene(tag))
    {
        releaseCurrentScene();
        _currentScene = scene;
        initCurrentScene();
    }
}

void SceneManager::changeScene(SceneTag tag, vector<int> args)
{
    //넘어온 씬이 존재한다면 & 모드나 맵같은 어떤 args들이 함께 넘어온다면
    if (auto scene = findScene(tag))
    {
        releaseCurrentScene();
        _currentScene = scene;
		_currentScene->handleArgs(args);
        initCurrentScene();
    }
}

void SceneManager::initCurrentScene()
{
    _currentScene->init();
}

void SceneManager::releaseCurrentScene()
{
    //현재 씬 해제
    _currentScene->release();
}

void SceneManager::updateCurrentScene()
{
    //현재 씬을 업데이트 해준다
    _currentScene->update();
}

void SceneManager::renderCurrentScene(HDC hdc)
{
    //현재 씬을 그려준다
    _currentScene->render(hdc);
}

Scene * SceneManager::findScene(SceneTag tag)
{
    auto key = _mSceneList.find(tag);
    if (key != _mSceneList.end())
    {
        return key->second;
    }
    return nullptr;
}

bool SceneManager::deleteScene(SceneTag tag)
{
	//cout << "삭제전" << endl;
	//for (auto s : _mSceneList)
	//{
	//	cout << static_cast<int>(s.first) << endl;
	//}
    auto key = _mSceneList.find(tag);
    if (key != _mSceneList.end())
    {
        _mSceneList.erase(key);
		//cout << "삭제후" << endl;
		//for (auto s : _mSceneList)
		//{
		//	cout << static_cast<int>(s.first) << endl;
		//}
        return true;
    }
    return false;
}

bool SceneManager::deleteAll()
{
    auto it = _mSceneList.begin();
    for (; it != _mSceneList.end(); )
    {
        if (it->second != NULL)
        {
            SAFE_DELETE(it->second);
            it = _mSceneList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    _mSceneList.clear();
    return true;
}
