#pragma once
//게임 오브젝트 태그
/* 
    새로 추가하면 반드시 gameObjectManager생성자에 render순서 결정할 것,
    showTagForDebug함수의 switch문에도 string추가하기 
*/
enum class GameObjectTag 
{
    Not,
    Player,
    Bomb,
    DesignElement,
    Wave,
    WaveStartingPoint,
	Item
};

//어떤 씬인지
enum class SceneTag
{
    Login, //제일 처음 화면
    ModeSelect, //모드 선택
    Lobby,
    Play //게임화면
};

//모드선택 태그
enum class ModeTypeTag
{
    Not,
    TwoPlayer,
    Monster
};

//맵 종류 태그
enum class MapTypeTag
{
    Not,
    Forest
};

enum class PlayerTypeTag
{
	Player1,
	Player2,
	SoloPlayer
};
//플레이어 상태
enum class PlayerStateTag
{
    Not,
    Ready,
    Wait,
    Up,
    Down,
    Left,
    Right,
    Trap,
    Die,
	Live,
	Jump
};

enum class BombStateTag
{
    Not,
    Set,
	MakeWaveStartingPoint,
    Pop
};

enum class WaveStartingPointStateTag
{
    Making,
    Complete
};

enum class WaveStateTag
{
    Up,
    Down,
    Left,
    Right
};

//아이템 종류 태크
enum class ItemTypeTag
{
	Not,
	Ballon,
	Potion,
	PotionMakePowerMax,
	Skate
};

//블럭 종류 태그
enum class BlockTypeTag
{
	MovableBlock, // 움직일 수 있는 블럭
	HidableBlock, // 숨을 수 있는 블럭
	BasicBlock, // 기본 블럭
};

//사운드 태그
enum class SoundTypeTag
{
	LoginScene,
	ModeSelectScene,
	LobbyScene,
	PlayScene,
	Click,
	PtInRect,
	GameStart,
	EatItem,
	Wave,
	Win,
	Lose,
	Logo,
	BombPop,
	BombSet,
	GameOver,
	PlayerDie
};