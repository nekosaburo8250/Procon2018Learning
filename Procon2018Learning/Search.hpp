﻿#pragma once

#include "Game.hpp"


class node
{
public:
	const static int NumCallPlay = 10000;
	const static int Threshold = 1000;

private:
	node *Parent;
	unsigned short NumChildren;
	node *Child[Max_ActionID][Max_ActionID];
	stage Stage;
	float Q;
	int N;
	int Record;
	team_no Team;

	int Play();
	int Selection();
	void Expansion();
	int Evaluation();
	int Rollout(stage Stage, int NumTurn);
	float UCB1(float Q, int NChild);
	bool IsLeafNode();
	void ClearChildNode();

public:
	node(node *Parent, stage &Stage, team_no Team);
	~node();
	void Search(int NumCallPlay, int(&Result)[Max_ActionID][Max_ActionID]);
	node* Deepen(action_id Action1, action_id Action2);
};