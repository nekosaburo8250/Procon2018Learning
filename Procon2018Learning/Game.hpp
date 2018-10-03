#pragma once

#include<array>
#include <random>


enum TeamNo
{
	None = -1,
	Team_1P,
	Team_2P,
	NumTeams
};

enum IntentionActioin
{
	IA_MoveAgent,
	IA_RemovePanel
};

enum PanelCheck
{
	PC_Unchecked,
	PC_Checked,
	PC_Set
};

class Panel
{
	int Point;

public:
	Panel();
	~Panel();

	void Init(int Point);
	void MakeCard(TeamNo Team);
	void RemoveCard();
	int GetScore();
	TeamNo GetState();
	void SetSurrounded(bool IsSurrounded, TeamNo Team);
	bool GetSurrounded(TeamNo Team);
};

class Agent
{
	TeamNo Team;
	int PositionX;
	int PositionY;

public:
	Agent();
	~Agent();

	void Init(int PositionX, int PositionY, TeamNo Team);
	void Move(int DeltaX, int DeltaY);
};

struct Intention
{
	int DeltaX;
	int DeltaY;
	IntentionActioin Action;
};

class Stage
{
	const static int MaxTurn = 60;
	const static int MaxX = 12;
	const static int MaxY = 12;
	const static int NumAgents = 2;
	int NumX;
	int NumY;
	Agent Agents[NumTeams][NumAgents];
	Panel Panels[MaxY][MaxX];
	int TileScore1P;
	int TileScore2P;
	int RegionScore1P;
	int RegionScore2P;

	std::random_device rand;

	int PanelPointRandom();
	void InitRandomStage();
	int UpdateRegionScore_Check(int x, int y, TeamNo Team, PanelCheck(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore_Set(int x, int y, TeamNo Team, bool Surrounded, PanelCheck(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore();
	void UpdateTileScore();

public:
	Stage();
	~Stage();

	void UpdateScore();
	void Action(Intention Intentions[]);
	bool CanAction(Intention Intentions[]);
};