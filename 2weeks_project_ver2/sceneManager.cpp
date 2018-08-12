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

    /* �� ��� */
    LoginScene* login = new LoginScene();
    SCENEMANAGER->registerScene(SceneTag::Login, login);

    ModeSelectScene* modeSelect = new ModeSelectScene;
    SCENEMANAGER->registerScene(SceneTag::ModeSelect, modeSelect);

	//�κ���� ��弱�þ����� �κ������ �Ѿ�� ���� �������� ����
	//�÷��̾��� �κ������ �÷��̾����� �Ѿ�� ���� �������� ����

    /* ���� ù �� ���� */
    _currentScene = login;
    initCurrentScene(); //�̰Ŷ��� �������°ſ��� �Ф�
}

//����
void SceneManager::release()
{
    deleteAll();
}

//���� ����Ѵ�
void SceneManager::registerScene(SceneTag tag, Scene* scene)
{
    _mSceneList.insert(make_pair(tag, scene));
}


void SceneManager::changeScene(SceneTag tag)
{
    //�Ѿ�� ���� �����Ѵٸ�
    if (auto scene = findScene(tag))
    {
        releaseCurrentScene();
        _currentScene = scene;
        initCurrentScene();
    }
}

void SceneManager::changeScene(SceneTag tag, vector<int> args)
{
    //�Ѿ�� ���� �����Ѵٸ� & ��峪 �ʰ��� � args���� �Բ� �Ѿ�´ٸ�
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
    //���� �� ����
    _currentScene->release();
}

void SceneManager::updateCurrentScene()
{
    //���� ���� ������Ʈ ���ش�
    _currentScene->update();
}

void SceneManager::renderCurrentScene(HDC hdc)
{
    //���� ���� �׷��ش�
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
	//cout << "������" << endl;
	//for (auto s : _mSceneList)
	//{
	//	cout << static_cast<int>(s.first) << endl;
	//}
    auto key = _mSceneList.find(tag);
    if (key != _mSceneList.end())
    {
        _mSceneList.erase(key);
		//cout << "������" << endl;
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
