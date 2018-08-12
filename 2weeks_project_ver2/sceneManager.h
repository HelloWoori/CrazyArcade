/*아직 짜다 말았음*/
#pragma once
#include "singletonBase.h"
#include "scene.h"

class SceneManager : public SingletonBase<SceneManager>
{
private:
    map<SceneTag, Scene*> _mSceneList;

    //Scene* _resevedScene; //다음 씬
    Scene* _currentScene; //현재 씬

public:
    SceneManager();
    ~SceneManager();

    void init();
    void release();

    void registerScene(SceneTag tag, Scene* scene);
    void changeScene(SceneTag tag);
    void changeScene(SceneTag tag, vector<int> args);
	bool deleteScene(SceneTag tag);

    void initCurrentScene();
    void releaseCurrentScene();
    void updateCurrentScene();
    void renderCurrentScene(HDC hdc);

    Scene* findScene(SceneTag tag);
    //BOOL deleteScene(SceneTag tag);
    bool deleteAll();
};