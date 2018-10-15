#include "Game.hpp"
#include "Random.hpp"
#include <iostream>
#include <windows.h>


int stage::PanelPointRandom()
{
	int abs = random::Mod(17);
	int Negative = random::Mod(10);
	return (Negative == 0) ? -abs : abs;
}

void stage::InitRandomStage()
{
	NumX = 11;//random::Mod(10) + 3;
	NumY = 12;//random::Mod(10) + 3;
	CntTurn = 0;
	TileScore1P = 0;
	TileScore2P = 0;
	RegionScore1P = 0;
	RegionScore2P = 0;

	int CopyType = random::Mod(2);
	int AgentX = random::Mod(NumX / 2);
	int AgentY = random::Mod(NumY / 2);

	if(CopyType == 0)
	{
		for(int y = 0; y < NumY; ++y)
		{
			for(int x = 0; x < NumX / 2; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int xInv = NumX - x - 1;
				int Score = Panels[y][x].GetScore();
				Panels[y][xInv].Init(Score);
			}
			if(NumX % 2 != 0)
			{
				Panels[y][NumX / 2].Init(PanelPointRandom());
			}
		}
		Agents[0][0].Init(AgentX, AgentY, Team_1P);
		Agents[0][1].Init(NumX - AgentX - 1, AgentY, Team_2P);
		Agents[1][0].Init(AgentX, NumY - AgentY - 1, Team_1P);
		Agents[1][1].Init(NumX - AgentX - 1, NumY - AgentY - 1, Team_2P);
	}
	else
	{
		for(int y = 0; y < NumY / 2; ++y)
		{
			for(int x = 0; x < NumX; ++x)
			{
				Panels[y][x].Init(PanelPointRandom());
				int yInv = NumY - y - 1;
				int Score = Panels[y][x].GetScore();
				Panels[yInv][x].Init(Score);
			}
		}
		if(NumY % 2 != 0)
		{
			for(int x = 0; x < NumX; ++x)
			{
				Panels[NumY / 2][x].Init(PanelPointRandom());
			}
		}
		Agents[0][0].Init(AgentX, AgentY, Team_1P);
		Agents[0][1].Init(AgentX, NumY - AgentY - 1, Team_1P);
		Agents[1][0].Init(NumX - AgentX - 1, AgentY, Team_2P);
		Agents[1][1].Init(NumX - AgentX - 1, NumY - AgentY - 1, Team_2P);
	}
	std::cout << "<===========================================>" << std::endl;
	PrintStage();
	std::cout << "<===========================================>" << std::endl;
}

int stage::UpdateRegionScore_Check(int x, int y, team_no Team, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX])
{
	if(Panels[y][x].GetState() == Team)
	{
		return 1;
	}
	if(CheckedPanel[Team][y][x] != PC_Unchecked)
	{
		return -1;
	}
	CheckedPanel[Team][y][x] = PC_Checked;
	if((x == 0) || (x == NumX - 1) || (y == 0) || (y == NumY - 1))
	{
		UpdateRegionScore_Set(x, y, Team, false, CheckedPanel);
		return 0;
	}

	int l = UpdateRegionScore_Check(x - 1, y, Team, CheckedPanel);
	if(l == 0)
	{
		return 0;
	}
	int t = UpdateRegionScore_Check(x, y + 1, Team, CheckedPanel);
	if(t == 0)
	{
		return 0;
	}
	int r = UpdateRegionScore_Check(x + 1, y, Team, CheckedPanel);
	if(r == 0)
	{
		return 0;
	}
	int b = UpdateRegionScore_Check(x, y - 1, Team, CheckedPanel);
	if(b == 0)
	{
		return 0;
	}
	if((l == -1 && t == -1) && (r == -1 && b == -1))
	{
		return -1;
	}
	return 1;
}

void stage::UpdateRegionScore_Set(int x, int y, team_no Team, bool Surrounded, panel_check(&CheckedPanel)[NumTeams][MaxY][MaxX])
{
	if(Panels[y][x].GetState() == Team)
	{
		Panels[y][x].SetSurrounded(false, Team);
	}
	if(CheckedPanel[Team][y][x] == PC_Set)
	{
		return;
	}
	CheckedPanel[Team][y][x] = PC_Set;
	Panels[y][x].SetSurrounded(Surrounded, Team);
	if(x > 0)
	{
		UpdateRegionScore_Set(x - 1, y, Team, Surrounded, CheckedPanel);
	}
	if(y > 0)
	{
		UpdateRegionScore_Set(x, y - 1, Team, Surrounded, CheckedPanel);
	}
	if(x < NumX - 1)
	{
		UpdateRegionScore_Set(x + 1, y, Team, Surrounded, CheckedPanel);
	}
	if(y < NumY - 1)
	{
		UpdateRegionScore_Set(x, y + 1, Team, Surrounded, CheckedPanel);
	}
}

void stage::UpdateRegionScore()
{
	panel_check CheckedPanel[NumTeams][MaxY][MaxX] = {};
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char y = 0; y < NumY; ++y)
		{
			for(char x = 0; x < NumX; ++x)
			{
				if(CheckedPanel[t][y][x] == 0)
				{
					int Ret = UpdateRegionScore_Check(x, y, t, CheckedPanel);
					if(Ret == 1)
					{
						UpdateRegionScore_Set(x, y, t, true, CheckedPanel);
					}
				}
			}
		}
	}
}

void stage::UpdateTileScore()
{
	RegionScore1P = 0;
	RegionScore2P = 0;
	for(int y = 0; y < NumY; ++y)
	{
		for(int x = 0; x < NumX; ++x)
		{
			int Score = Panels[y][x].GetScore();
			switch(Panels[y][x].GetState())
			{
			case Team_1P:
				RegionScore1P += Score;
				break;

			case Team_2P:
				RegionScore2P += Score;
				break;
			}
		}
	}
}

stage::stage()
{
	//InitRandomStage();
}

stage::~stage()
{

}

void stage::Action(intention(&Intentions)[NumTeams][NumAgents])
{
	bool CanActionFlags[NumTeams][NumAgents];
	CanAction(Intentions, CanActionFlags);
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			if(!CanActionFlags[t][a])
			{
				continue;
			}
			intention &Intention = Intentions[t][a];
			if(Intention.Action == IA_MoveAgent)
			{
				Agents[t][a].Move(Intention.DeltaX, Intention.DeltaY);
				position NextPosition = Agents[t][a].GetPosition();
				Panels[NextPosition].MakeCard(t);
			}
			else
			{
				Panels[Agents[t][a].GetPosition()].RemoveCard();
			}
		}
	}
	UpdateScore();
}

void stage::Action(action_id(&IntentionIDs)[NumTeams][NumAgents])
{
	intention Intentions[NumTeams][NumAgents];
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			Intentions[t][a] = IntentionIDs[t][a];
		}
	}
	Action(Intentions);
}

void stage::CanAction(intention(&Intentions)[NumTeams][NumAgents], bool(&Result)[NumTeams][NumAgents])
{
	position ExpectedPositions[NumTeams][NumAgents];
	can_action_flag Flags[NumTeams][NumAgents];
	//ステージ1・座標外への移動意思、留まる意思に対して決定
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			position Position = Agents[t][a].GetPosition();
			intention Intention = Intentions[t][a];
			ExpectedPositions[t][a] = Position << Intention;
			Flags[t][a] = 0;
			switch(CanActionOne(Position, Intention))
			{
			case -1:
				Flags[t][a] = F_Decided;
				break;

			case 1:
				Flags[t][a] = F_Decided | F_CanAction;
				break;
			}
		}
	}
	//ステージ2・目標の座標の重複,またはエージェントか敵タイルがあるパネルへの移動に対して決定
	bool Loop;
	do
	{
		Loop = false;
		for(team_no t = 0; t < NumTeams; ++t)
		{
			for(char a = 0; a < NumAgents; ++a)
			{
				if(Flags[t][a] & F_Decided)
				{
					continue;
				}
				for(team_no tt = 0; tt < NumTeams; ++tt)
				{
					for(char aa = 0; aa < NumAgents; ++aa)
					{
						if(t == tt && a == aa)
						{
							continue;
						}
						if(ExpectedPositions[t][a] == ExpectedPositions[tt][aa])
						{
							Flags[t][a] = F_Decided;
							Flags[tt][aa] = F_Decided;
							ExpectedPositions[t][a] = Agents[t][a].GetPosition();
							ExpectedPositions[tt][aa] = Agents[tt][aa].GetPosition();
							Loop = true;
						}
					}
				}
				if(Intentions[t][a].Action == IA_MoveAgent)
				{
					team_no EnemyTeam = t == Team_1P ? Team_2P : Team_1P;
					if(Panels[ExpectedPositions[t][a]].GetState() == EnemyTeam)
					{
						Flags[t][a] = F_Decided;
						ExpectedPositions[t][a] = Agents[t][a].GetPosition();
						Loop = true;
					}
				}
				Result[t][a] = (Flags[t][a] & F_Decided) != 0;
			}
		}
	} while(Loop);
	for(team_no t = 0; t < NumTeams; ++t)
	{
		for(char a = 0; a < NumAgents; ++a)
		{
			Result[t][a] = !(Flags[t][a] & F_Decided) || (Flags[t][a] & F_CanAction);
		}
	}
}

bool stage::CanAction(intention(&Intentions)[NumTeams][NumAgents])
{
	bool Result[NumTeams][NumAgents];
	CanAction(Intentions, Result);
	return (Result[0][0] && Result[0][1]) && (Result[1][0] && Result[1][1]);
}

void stage::Action(intention(&Intentions)[NumAgents], team_no Team)
{
	if(!CanAction(Intentions))
	{
		return;
	}
	for(char a = 0; a < NumAgents; ++a)
	{
		intention &Intention = Intentions[a];
		if(Intention.Action == IA_MoveAgent)
		{
			Agents[Team][a].Move(Intention.DeltaX, Intention.DeltaY);
			position NextPosition = Agents[Team][a].GetPosition();
			Panels[NextPosition].MakeCard(Team);
		}
		else
		{
			Panels[Agents[Team][a].GetPosition()].RemoveCard();
		}
	}
	UpdateTileScore();
}

void stage::Action(action_id(&IntentionIDs)[NumTeams], team_no Team)
{
	intention Intentions[NumTeams];
	for(char a = 0; a < NumAgents; ++a)
	{
		Intentions[a] = IntentionIDs[a];
	}
	Action(Intentions, Team);
}

bool stage::CanAction(intention(&Intentions)[NumAgents])
{
	if(!(CanActionOne(Agents[Team_1P][0].GetPosition(), Intentions[0]) && CanActionOne(Agents[Team_1P][1].GetPosition(), Intentions[1])))
	{
		return false;
	}
	return Agents[Team_1P][0].GetPosition() << Intentions[0] != Agents[Team_1P][1].GetPosition() << Intentions[1];
}

bool stage::CanAction(intention &Intention, team_no Team, char AgentNo)
{
	return CanActionOne(Agents[Team][AgentNo].GetPosition(), Intention) != -1;
}

char stage::CanActionOne(position Position, intention Intention)
{
	if(Intention.DeltaX == 0 && Intention.DeltaY == 0)
	{
		return 1;
	}
	Intention.Action = IA_MoveAgent;
	Position += Intention;
	return (0 <= Position.x && Position.x < NumX) && (0 <= Position.y && Position.y < NumY) ? 0 : -1;
}

void stage::UpdateScore()
{
	UpdateRegionScore();
	UpdateTileScore();
}

int stage::GetNumX()
{
	return NumX;
}

int stage::GetNumY()
{
	return NumY;
}

int stage::GetCntTurn()
{
	return CntTurn;
}

int stage::GetScore1P()
{
	//std::cout << "Tile:" << TileScore1P << "Region" << RegionScore1P << std::endl;
	return TileScore1P + RegionScore1P;
}

int stage::GetScore2P()
{
	return TileScore2P + RegionScore2P;
}

agent* stage::GetAgent(team_no Team, int AgentNo)
{
	return &Agents[Team][AgentNo];
}

void stage::PrintStage()
{
	using namespace std;

	for(int y = 0; y < NumY; y++)
	{
		for(int x = 0; x < NumX; x++)
		{
			printf("%3d ", Panels[y][x].GetScore());
		}
		cout << endl;
	}
	cout << endl;
	for (int y = 0; y < NumY; y++)
	{
		for (int x = 0; x < NumX; x++)
		{
			if (Agents[0][0].GetPosition().x == x && Agents[0][0].GetPosition().y == y) { cout << "●"; continue; }
			if (Agents[0][1].GetPosition().x == x && Agents[0][1].GetPosition().y == y) { cout << "■"; continue; }
			if (Agents[1][0].GetPosition().x == x && Agents[1][0].GetPosition().y == y) { cout << "○"; continue; }
			if (Agents[1][1].GetPosition().x == x && Agents[1][1].GetPosition().y == y) { cout << "□"; continue; }

			cout << "−";
		}
		cout << endl;
	}
	cout << endl;
	for(int y = 0; y < NumY; y++)
	{
		for(int x = 0; x < NumX; x++)
		{
			switch(Panels[y][x].GetState())
			{
			case None:
				if(Panels[y][x].GetSurrounded(Team_1P))
				{
					cout << "1";
				}
				else
				{
					cout << "-";
				}
				if(Panels[y][x].GetSurrounded(Team_2P))
				{
					cout << "2";
				}
				else
				{
					cout << "-";
				}
				break;

			case Team_1P:
				cout << "■";
				break;

			case Team_2P:
				cout << "□";
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}

void stage:: color(int a, int b, int c, int d)
{
	HANDLE hStdout;
	WORD wAttributes;
	char txt;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbi);

	txt = a; //タイトルの文字色

	wAttributes = txt;
	wAttributes = wAttributes | b; // 文字強調 
	wAttributes = wAttributes | c; // タイトルの背景色 
	wAttributes = wAttributes | d; // 背景色強調
	SetConsoleTextAttribute(hStdout, wAttributes); // SET
	return;
}