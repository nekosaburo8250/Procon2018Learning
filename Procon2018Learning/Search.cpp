#include <iostream>
#include <random>
#include "Search.hpp"


bool node::CanMove(action_id No, position Position)
{
	switch(No)
	{
	case Stay:
		return true;

	case Move_TopLeft:
	case Remove_TopLeft:
		return Position.x > 0 && Position.y > 0;

	case Move_Top:
	case Remove_Top:
		return Position.y > 0;

	case Move_TopRight:
	case Remove_TopRight:
		return Position.x < Stage.GetNumX() - 1 && Position.y > 0;

	case Move_Left:
	case Remove_Left:
		return Position.x > 0;

	case Move_Right:
	case Remove_Right:
		return Position.x < Stage.GetNumX() - 1;

	case Move_BottomLeft:
	case Remove_BottomLeft:
		return Position.x > 0 && Position.y < Stage.GetNumY() - 1;

	case Move_Bottom:
	case Remove_Bottom:
		return Position.y < Stage.GetNumY() - 1;

	case Move_BottomRight:
	case Remove_BottomRight:
		return Position.x < Stage.GetNumX() - 1 && Position.y < Stage.GetNumY() - 1;

	default:
		return false;
	}
}

node::node(node *Parent, stage &Stage)
{
	this->Parent = Parent;
	this->Stage = Stage;
}

node::~node()
{
	int NumChild = (int)Child.size();
	for(int i = 0; i < NumChild; ++i)
	{
		delete Child[i];
		Child[i] = nullptr;
	}
	Child.clear();
}

void node::Search(stage stage)
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				intention Intention1;
				intention Intention2;
				switch(k)
				{
				case 0:
					Intention1.Action = IA_MoveAgent;
					Intention2.Action = IA_MoveAgent;
				case 1:
					Intention1.Action = IA_MoveAgent;
					Intention2.Action = IA_RemovePanel;
				case 2:
					Intention1.Action = IA_RemovePanel;
					Intention2.Action = IA_MoveAgent;
				case 3:
					Intention1.Action = IA_RemovePanel;
					Intention2.Action = IA_RemovePanel;
				}
				switch(i)
				{
				case 0:
					Intention1.DeltaX = 0;
					Intention1.DeltaY = 1;
				case 1:
					Intention1.DeltaX = 1;
					Intention1.DeltaY = 1;
				case 2:
					Intention1.DeltaX = 1;
					Intention1.DeltaY = 0;
				case 3:
					Intention1.DeltaX = 1;
					Intention1.DeltaY = -1;
				case 4:
					Intention1.DeltaX = 0;
					Intention1.DeltaY = -1;
				case 5:
					Intention1.DeltaX = -1;
					Intention1.DeltaY = -1;
				case 6:
					Intention1.DeltaX = -1;
					Intention1.DeltaY = 0;
				case 7:
					Intention1.DeltaX = -1;
					Intention1.DeltaY = 1;
				}
				switch(j)
				{
				case 0:
					Intention2.DeltaX = 0;
					Intention2.DeltaY = 1;
				case 1:
					Intention2.DeltaX = 1;
					Intention2.DeltaY = 1;
				case 2:
					Intention2.DeltaX = 1;
					Intention2.DeltaY = 0;
				case 3:
					Intention2.DeltaX = 1;
					Intention2.DeltaY = -1;
				case 4:
					Intention2.DeltaX = 0;
					Intention2.DeltaY = -1;
				case 5:
					Intention2.DeltaX = -1;
					Intention2.DeltaY = -1;
				case 6:
					Intention2.DeltaX = -1;
					Intention2.DeltaY = 0;
				case 7:
					Intention2.DeltaX = -1;
					Intention2.DeltaY = 1;
				}

			}
		}
	}
	Child.clear();
}

void node::Selection()
{

}

void node::Expansion(team_no Team)
{
	position Agent1Pos = Stage.GetAgent(Team, 0)->GetPosition();
	position Agent2Pos = Stage.GetAgent(Team, 0)->GetPosition();

	for (int i = 0; i < 17; ++i)
	{
		if (!CanMove((action_id)i, Agent1Pos))
		{
			continue;
		}
		for (int j = 0; j < 17; ++j)
		{
			if (!CanMove((action_id)j, Agent2Pos))
			{
				continue;
			}

			node *NewNode = new node(this, Stage);
			if (Team == Team_1P)
			{
				NewNode->Intention1 = (action_id)i;
				NewNode->Intention2 = (action_id)j;
			}
			else
			{
				NewNode->Intention1 = None;
				NewNode->Intention2 = None;
			}
			NewNode->Team = (Team == Team_1P) ? Team_2P : Team_1P;
			Child.push_back(NewNode);
		}
	}
}

void node::Evaluation()
{

}

bool node::IsLeafNode()
{
	return Child.size() == 0;
}

bool node::Rollout(stage Stage, int turn)
{
	for(int i = turn; i > 0; i--)
	{
		intention Intentions[4];
		for(int i = 0; i < 4; ++i)
		{
			Intentions[i].DeltaX = rand() % 3 - 1;
			Intentions[i].DeltaY = rand() % 3 - 1;
		}
		Stage.Action(Intentions);
	}
}