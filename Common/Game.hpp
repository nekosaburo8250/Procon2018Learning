#pragma once

#include "Intention.hpp"
#include "Panel.hpp"
#include "Agent.hpp"
#include "Position.hpp"
#include <string>
#include <vector>

typedef unsigned char can_action_flag;
enum
{
	F_Decided = 0x01,
	F_CanAction = 0x02,
	F_Move = 0x04
};

typedef char stage_initialize_flag;
enum
{
	STAGE_INIT_RANDOM,
	STAGE_INIT_BINARYFILE
};

const static unsigned char MaxX = 12;
const static unsigned char MaxY = 12;

class panels
{
	panel Panels[MaxY][MaxX];

public:
	panel* operator[](unsigned char y)
	{
		return Panels[y];
	}
	const panel* operator[](unsigned char y)const
	{
		return Panels[y];
	}
	panel& operator[](position p)
	{
		return Panels[p.y][p.x];
	}
	const panel& operator[](position p)const
	{
		return Panels[p.y][p.x];
	}
};

class stage
{
public:
	const static int MaxTurns = 80;
	const static int NumAgents = 2;

private:
	typedef char panel_check;
	enum
	{
		PC_Unchecked,
		PC_Checked,
		PC_Set
	};
	struct intention_info
	{
		intention Delta;
		position ExpectedPosition;
		position NextPosition;
		char CanAct;
	};
	panels Panels;
	agent Agents[NumTeams][NumAgents];
	unsigned char NumX;
	unsigned char NumY;
	unsigned char CntTurn;
	short TileScore1P;
	short TileScore2P;
	short RegionScore1P;
	short RegionScore2P;

	int PanelPointRandom();
	void BinaryStage();
	void TextStage(std::string text);
	std::vector<std::string>split(std::string str, char sp);
	
	int UpdateRegionScore_Check(int x, int y, team_no Team, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore_Set(int x, int y, team_no Team, bool Surrounded, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX]);
	void UpdateRegionScore();
	void UpdateTileScore();

	bool Move(intention_info(&Infos)[NumTeams][NumAgents], team_no t, char a)const;

public:
	stage();
	stage(stage_initialize_flag Flag);
	stage(const char *QRCodeString);
	stage(std::string QRtext);
	~stage();

	void InitRandomStage();
	//両チーム用
	void Action(intention(&Intentions)[NumTeams][NumAgents]);
	void Action(action_id(&IntentionIDs)[NumTeams][NumAgents]);
	void CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])const;
	void CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])const;
	bool CanAction(intention(&Intentions)[NumTeams][NumAgents])const;
	bool CanAction(action_id(&IntentionIDs)[NumTeams][NumAgents])const;

	//1チーム用
	bool CanAction(intention(&Intentions)[NumAgents], team_no Team)const;
	bool CanAction(action_id(&IntentionIDs)[NumAgents], team_no Team)const;

	//1プレイヤー用
	bool CanAction(intention Intention, team_no Team, char AgentNo)const;
	bool CanAction(action_id IntentionID, team_no Team, char AgentNo)const;
	char CanActionOne(position Position, intention Intention)const;

	void UpdateScore();
	unsigned char GetNumX()const;
	unsigned char GetNumY()const;
	unsigned char GetCntTurn()const;
	short GetScore1P()const;
	short GetScore2P()const;
	//agent& GetAgent(team_no Team, char AgentNo);
	const agent& GetAgent(team_no Team, char AgentNo)const;
	const panels& GetPanels()const;
	void PrintStage()const;
	void SetState(int x,int y,int state);
};