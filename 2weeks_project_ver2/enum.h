#pragma once
//���� ������Ʈ �±�
/* 
    ���� �߰��ϸ� �ݵ�� gameObjectManager�����ڿ� render���� ������ ��,
    showTagForDebug�Լ��� switch������ string�߰��ϱ� 
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

//� ������
enum class SceneTag
{
    Login, //���� ó�� ȭ��
    ModeSelect, //��� ����
    Lobby,
    Play //����ȭ��
};

//��弱�� �±�
enum class ModeTypeTag
{
    Not,
    TwoPlayer,
    Monster
};

//�� ���� �±�
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
//�÷��̾� ����
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

//������ ���� ��ũ
enum class ItemTypeTag
{
	Not,
	Ballon,
	Potion,
	PotionMakePowerMax,
	Skate
};

//�� ���� �±�
enum class BlockTypeTag
{
	MovableBlock, // ������ �� �ִ� ��
	HidableBlock, // ���� �� �ִ� ��
	BasicBlock, // �⺻ ��
};

//���� �±�
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