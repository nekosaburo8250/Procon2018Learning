#include "BattleField.hpp"
#include <iostream>


battle_field::battle_field()
{

}

battle_field::~battle_field()
{

}

void battle_field::Battle(int NumTurn)
{
	int t = 0;
	char isContinue;
	stage Stage;
	friend_node *CurrentNode = new friend_node(Stage, NumTurn);
	Stage.PrintStage();
	do
	{
		using namespace std;
		int Max = 0;
		action_id UserIntentions[NumTeams][stage::NumAgents] = { { -1,-1 },{ -1,-1 } };
		action_id IntentionIDs[NumTeams][stage::NumAgents] = { {-1,-1},{-1,-1} };
		int Result[ID_MaxID][ID_MaxID];

		cout << "================================================================" << endl;
		cout << "Action" << t << endl;

		CurrentNode->Search(node::NumCallPlay);
		CurrentNode->Result(Result);

		Stages[t] = Stage;
		CurrentNodes[t] = CurrentNode;

		for (action_id i = 0; i < ID_MaxID; ++i)
		{
			for (action_id j = 0; j < ID_MaxID; ++j)
			{
				if (Max < Result[i][j])
				{
					Max = Result[i][j];
					IntentionIDs[Team_1P][0] = i;
					IntentionIDs[Team_1P][1] = j;
				}
			}
		}

		{
			intention Intention1P_1 = IntentionIDs[Team_1P][0];
			intention Intention1P_2 = IntentionIDs[Team_1P][1];
			cout << "1P-1  x : " << (int)Intention1P_1.DeltaX << " y : " << (int)Intention1P_1.DeltaY << endl;
			cout << "ID:[ " << intoCard((int)IntentionIDs[Team_1P][0]) << " ]" << endl;
			cout << "1P-2  x : " << (int)Intention1P_2.DeltaX << " y : " << (int)Intention1P_2.DeltaY << endl;
			cout << "ID:[ " << intoCard((int)IntentionIDs[Team_1P][1]) << " ]" << endl;
		}
		if (CurrentNode == nullptr) { cout << "null1" << endl; }
		if (CurrentNode->ChildNode(IntentionIDs[Team_1P]) == nullptr) { cout << "null2" << endl; }
		CurrentNode->ChildNode(IntentionIDs[Team_1P])->Search(node::NumCallPlay);
		CurrentNode->ChildNode(IntentionIDs[Team_1P])->Result(Result);
		Max = 0;
		for (action_id i = 0; i < ID_MaxID; ++i)
		{
			for (action_id j = 0; j < ID_MaxID; ++j)
			{
				if (Max < Result[i][j])
				{
					Max = Result[i][j];
					IntentionIDs[Team_2P][0] = i;
					IntentionIDs[Team_2P][1] = j;
				}
			}
		}

		{
			intention Intention2P_1 = IntentionIDs[Team_2P][0];
			intention Intention2P_2 = IntentionIDs[Team_2P][1];
			//CurrentNode->ChildNode(IntentionIDs[Team_1P])->PrintChildNodeInfo();
			cout << "2P-1 : x : " << (int)Intention2P_1.DeltaX << " y : " << (int)Intention2P_1.DeltaY << endl;
			cout << "ID: [" << intoCard((int)IntentionIDs[Team_2P][0]) << " ]" << endl;
			cout << "2P-2 : x : " << (int)Intention2P_2.DeltaX << " y : " << (int)Intention2P_2.DeltaY << endl;
			cout << "ID: [" << intoCard((int)IntentionIDs[Team_2P][1]) << " ]" << endl;
		}

		cout << "=====================Input intentionIDs =====================" << endl;
		cout << "7 8 9" << endl;
		cout << "4 5 6" << endl;
		cout << "1 2 3" << endl;
		bool Results[NumTeams][stage::NumAgents];
		for (int i = 0; i < NumTeams; i++)
		{
			cout << "Team" << i + 1 << "P:" << endl;
			for (int j = 0; j < stage::NumAgents; j++)
			{

				int UserIntention;
				cout << "Agent" << j + 1 << ":" << endl;
				cin >> UserIntention;
				UserIntentions[(i == 0) ? Team_1P : Team_2P][j] = (action_id)intoID(UserIntention);
			}
		}
		cout << "=====================End Input intentionIDs =====================" << endl;
		Stage.CanAction(UserIntentions, Results);
		for (int i = 0; i < NumTeams; i++)
		{
			for (int j = 0; j < stage::NumAgents; j++)
			{
				if (!Results[i][j]) { UserIntentions[(i == 0) ? Team_1P : Team_2P][j] = (action_id)0; }
			}
		}


		CurrentNode = CurrentNode->UpdateCurrentNode(UserIntentions);

		Stage.Action(UserIntentions);
		std::cout << "Turn" << (int)Stage.GetCntTurn() << endl;

		cout << "Stage;" << endl;
		Stage.PrintStage();
		cout << "Node" << endl;
		CurrentNode->PrintStage();
		cout << "1PScore : " << Stage.GetScore1P() << endl;
		cout << "2PScore : " << Stage.GetScore2P() << endl;

		t++;

		cout << "================Redo or Continue or End===================" << endl;
		do
		{
			cout << "0で終了,その他で続行,rで元に戻す,vで裏返す" << endl;
			cin >> isContinue;
			if (isContinue == 'v')
			{
				int x, y, state;
				cout << "x,y,state(neutral:-1 friendly:0 enemy:1):" << endl;
				cin >> x >> y >> state;
				Stage.SetState(x, y, state);
				friend_node *NewCurrentNode = new friend_node(Stage, NumTurn);
				CurrentNode = NewCurrentNode;
			}
			if (isContinue == 'r' && t > 0)
			{
				t--;
				Stage = Stages[t];
				CurrentNode = CurrentNodes[t];
				cout << "Stage:" << endl;
				Stage.PrintStage();
				cout << "Node:" << endl;
				CurrentNode->PrintStage();
			}
		} while (isContinue == 'r' && t > 0);

	} while (isContinue != '0');
	delete CurrentNode;
	CurrentNode = nullptr;
}

int battle_field::intoCard(action_id _IntentinoID)
{
	switch (_IntentinoID)
	{
	case 1:
		return 7;

	case 2:
		return 8;

	case 3:
		return 9;

	case 4:
		return 4;

	case 5:
		return 6;

	case 6:
		return 1;

	case 7:
		return 2;

	case 8:
		return 3;

	default:
		return 5;
	}
}

int battle_field::intoID(int _Card)
{
	switch (_Card)
	{
	case 7:
		return 1;

	case 8:
		return 2;

	case 9:
		return 3;

	case 4:
		return 4;

	case 6:
		return 5;

	case 1:
		return 6;

	case 2:
		return 7;

	case 3:
		return 8;

	default:
		return 0;
	}
}