#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"
#include <vector>

using namespace std;

class MyStrategy : public Strategy {
public:
	void Move(model::Tank self, model::World world, model::Move& move);

	//bonuses
	int GetNearestBonus(model::Tank &self, vector<model::Bonus> &bonuses);
	int GetNearestBonusByType(model::Tank &self, vector<model::Bonus> &bonuses, model::BonusType type);

	//opponents
	int GetNearestTankByAngle(model::Tank &self, vector<model::Tank> &tanks, double &angleToTarget /*out*/);
	int GetNearestTankByDistance(model::Tank &self, vector<model::Tank> &tanks, double &distanceToTarget /*out*/);
	int GetTankWithMinHP(vector<model::Tank> &tanks, int &targetHP /*out*/);
	int GetTankWithMaxHP(vector<model::Tank> &tanks, int &targetHP /*out*/);

	model::TankType SelectTank(int tank_index, int team_size);

private:
	double currentSpeed; //pixels pre tick
	
};

#endif
